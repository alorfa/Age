#pragma once

#include "ForwardRenderer.hpp"
#include "Age/Other/Logger.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Resource/ShaderLoader.hpp"
#include "Age/LL/Buffers/VertexBuffer.hpp"
#include "Age/egd.hpp"
#include "Age/Scene/Scene.hpp"

namespace a_game_engine
{
	ForwardRenderer::ForwardRenderer()
	{
		shader = &egd.shaders.loadPostproc(egd.res / "shader/postproc.pasl");
		mainFb.textures.resize(1);
	}
	void ForwardRenderer::clear()
	{
		mainFb.textures[0].destroy();
	}
	void ForwardRenderer::updateSize(const uvec2& newSize)
	{
		size = newSize;
		ImageInfo info{newSize, nullptr, TextureFormat::RGB_Float16};
		mainFb.createRenderBuffer(newSize);
		mainFb.textures[0].create({ info, false });
		mainFb.textures[0].setFiltering(TextureFiltering::Linear);
		mainFb.textures[0].setWrap(TextureWrap::ClampToEdge);
		mainFb.create();
	}
	void ForwardRenderer::drawObject(const Node& o, const SceneInfo& info)
	{
		o.forEachConst([&](const Node& n) {
			n.draw(info);
		});
	}
	void ForwardRenderer::drawScene(const Scene& sc, const Camera& camera)
	{
		ShaderSettings::Forward settings;
		SceneInfo info;
		info.camera = &camera;
		info.props.push_back({ "skybox", SkyBox::getSlot() });
		info.addLights(*sc.rootNode);
		settings.dirLights = info.lights.dir;
		settings.pointLights = info.lights.point;
		settings.spotLights = info.lights.spot;
		info.shaderSettings = settings;
		info.drawingCondition = [](const Material& m) {
			return m.shader->opaque;
			};
		SceneInfo transparentInfo = info;
		transparentInfo.drawingCondition = [](const Material& m) {
			return !m.shader->opaque;
			};
		settings.definesIndex = 0;
		transparentInfo.shaderSettings = settings;
		SceneInfo transparentSpecInfo = transparentInfo;
		settings.definesIndex = 1;
		transparentSpecInfo.shaderSettings = settings;

		Pipeline::set3DContext();
		mainFb.use();
		Pipeline::setBlendMode(BlendMode::Lerp);
		Pipeline::clear({ 0.1f, 0.1f, 0.1f }, true, false);
		sc.skyBox.cubemap->activate(SkyBox::getSlot());
		drawObject(*sc.rootNode, info);
		sc.skyBox.draw(camera, nullptr);

		sc.rootNode->forEachConst([&](const Node& n)
			{
				Pipeline::setBlendMode(BlendMode::Lerp);
				n.draw(transparentInfo);
				Pipeline::setBlendMode(BlendMode::Add);
				n.draw(transparentSpecInfo);
			});
		Pipeline::setBlendMode(BlendMode::Lerp);

		mainFb.useDefault(size);
		Pipeline::set2DContext();
		auto* verts = &VertexBuffer::getDefFramebuf();
		shader->use();
		shader->setUniform(shader->getLocation("tex"), mainFb.textures[0], 0);
		verts->draw();
	}
}