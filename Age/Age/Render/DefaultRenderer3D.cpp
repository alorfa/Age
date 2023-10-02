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
		mainFb.textures.resize(1);
	}
	void DefaultRenderer3D::updateSize(const uvec2& newSize)
	{
		size = newSize;
		ImageInfo info{newSize, nullptr, TextureFormat::RGB_Float16};
		mainFb.createRenderBuffer(newSize);
		mainFb.textures[0].create({ info, false });
		mainFb.textures[0].setFiltering(TextureFiltering::Linear);
		mainFb.textures[0].setWrap(TextureWrap::ClampToEdge);
		mainFb.create();
	}
	void DefaultRenderer3D::drawObject(const Node3D& o, const Scene3DInfo& info)
	{
		static mat4 identity;
		o.draw(identity, info);
	}
	void DefaultRenderer3D::drawScene(const Scene3D& sc, const Camera3D& camera)
	{
		Scene3DInfo info;
		info.camera = &camera;
		info.props.push_back({ "skybox", SkyBox::getSlot() });
		info.addLights(*sc.rootNode);
		ShaderSettings::Deferred deferred;
		deferred.bindings = { 4, 4, 3 };
		deferred.paintingFuncIndex = 0;
		ShaderSettings::Forward settings;
		settings.dirLights = info.lights.dir;
		settings.pointLights = info.lights.point;
		settings.spotLights = info.lights.spot;
		info.shaderSettings = settings;

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
		shader->setUniform(shader->getLocation("tex"), mainFb.textures[0], 0);
		verts->draw();
	}
}