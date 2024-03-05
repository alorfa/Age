#pragma once

#include "ForwardRenderer.hpp"
#include "Age/Other/Logger.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Resource/ShaderLoader.hpp"
#include "Age/LL/Buffers/VertexBuffer.hpp"
#include "Age/egd.hpp"
#include "Age/Scene/Scene.hpp"
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	ForwardRenderer::ForwardRenderer()
	{
		shader = &egd.shaders.loadPostproc(egd.res / "shader/postproc.pasl");
		mainFb.setTexturesCount(1);
	}
	void ForwardRenderer::clear()
	{
		colorBuffer.destroy();
		depthBuffer.destroy();
		mainFb.removeRenderBuffer();
	}
	void ForwardRenderer::updateSize(const uvec2& newSize)
	{
		size = newSize;
		Sampler2DInfo sampler{ TextureFiltering::Near };
		ImageInfo info{newSize, TextureFormat::RGB_F16};
		ImageInfo depthInfo{newSize, TextureFormat::Depth24};
		//mainFb.createRenderBuffer(newSize, TextureFormat::Depth24);
		Texture2D::Settings colorSettings{ info, TextureFormat::Auto, sampler, 1 };
		colorBuffer.create(colorSettings);
		//depthBuffer.create({ depthInfo, false });
		mainFb.setTexture(0, colorBuffer);
		//mainFb.setDepthTexture(depthBuffer);
		mainFb.createRenderBuffer(newSize, TextureFormat::Depth24);
		bloom.create(newSize / 2u);
		bloom.radius = 3.5f;
	}
	void ForwardRenderer::drawScene(const Scene& sc, const Camera& camera, float delta)
	{
		ShaderSettings::Forward settings;
		SceneInfo info;
		info.camera = &camera;
		info.props.push_back({ "diffuseMap", 10 });
		info.props.push_back({ "specularMap", 11 });
		info.props.push_back({ "brdfLut", 12 });
		info.props.push_back({ "maxSpecMipLevel", float(TexEnums::getLastMipLevel(env->specular.getSize()))});
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

		env->diffuse.activate(10);
		env->specular.activate(11);
		TextureLoader::getBrdfLut().activate(12);

		sc.rootNode->forEachConst([&](const Node& n) {
			n.draw(info);
		});
		skyBox.draw(camera, nullptr);

		sc.rootNode->forEachConst([&](const Node& n)
			{
				Pipeline::setBlendMode(BlendMode::Lerp);
				n.draw(transparentInfo);
				Pipeline::setBlendMode(BlendMode::Add);
				n.draw(transparentSpecInfo);
			});
		Pipeline::setBlendMode(BlendMode::Lerp);

		bloom.useDownscale(colorBuffer, 0);
		bloom.useUpscale(2, 0);

		const vec3 midColor = bloom.getTextures().crbegin()->getMidColor();
		const float brightness = vec3::dot(midColor, Math::LUMA);
		const float clampedBr = Math::lerp(brightness, 0.6f, 0.3f);
		const float curExp = 0.3f / clampedBr;
		exposure = Math::smooth(exposure, curExp, delta * 3.f);

		mainFb.useDefault(size);
		Pipeline::set2DContext();
		auto* verts = &VertexBuffer::getDefFramebuf();
		vec2 bloomRadius = {
			1.f / colorBuffer.getSize().x, 1.f / colorBuffer.getSize().y
		};
		shader->use();
		shader->setUniform(shader->getLocation("tex"), colorBuffer, 0);
		shader->setUniform(shader->getLocation("bloomTex"), bloom.getTextures()[0], 1);
		shader->setUniform(shader->getLocation("exposure"), exposure);
		shader->setUniform(shader->getLocation("bloomStrength"), 0.05f);
		shader->setUniform(shader->getLocation("bloomRadius"), bloomRadius * bloom.radius);
		verts->draw();
	}
}