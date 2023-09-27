#pragma once

#include "DefaultRenderer3D.hpp"
#include "Age/Resource/Logger.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Resource/ShaderLoader.hpp"
#include "Age/LL/Buffers/VertexBuffer.hpp"
#include "Age/Object/Object3D.hpp"
#include "Age/egd.hpp"
#include "Age/Scene/Scene3D.hpp"

namespace a_game_engine
{
	DefaultRenderer3D::DefaultRenderer3D()
	{
		shader = &egd.shaders.loadPostproc(egd.res / "shader/postproc.pasl");
	}
	void DefaultRenderer3D::updateSize(const uvec2& newSize)
	{
		size = newSize;
		mainFb.create(FrameBuffer2D::Settings(newSize, TextureFormat::RGB_Float16));
		mainFb.texture.setFiltering(TextureFiltering::Linear);
		mainFb.texture.setWrap(TextureWrap::ClampToEdge);
	}
	void DefaultRenderer3D::drawObject(const Node3D& o, const Scene3DInfo& info)
	{
		static mat4 identity;
		o.draw(identity, info);
		o.forEachConst([&](const Node3D& n)
			{
				drawObject(n, info);
			});
	}
	void DefaultRenderer3D::drawScene(const Scene3D& sc, const Camera3D& camera)
	{
		Scene3DInfo info;
		info.camera = &camera;
		info.props.push_back({ "skybox", SkyBox::getSlot() });

		Pipeline::set3DContext();
		mainFb.use();
		Pipeline::clear({ 0.1f, 0.1f, 0.1f });
		sc.skyBox.cubemap->activate(SkyBox::getSlot());
		sc.rootNode->forEachConst([&](const Node3D& n) {
			drawObject(n, info);
			});
		sc.skyBox.draw(camera, nullptr);

		mainFb.useDefault(size);
		Pipeline::set2DContext();
		auto* verts = &VertexBuffer::getDefFramebuf();
		shader->use();
		shader->setUniform(shader->getLocation("tex"), mainFb.texture, 0);
		verts->draw();
	}
}