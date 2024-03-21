#include "DeferredRenderer.hpp"
#include "Age/egd.hpp"
#include "Age/Object/Node.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Scene/SkyBox.hpp"
#include "Age/Scene/Scene.hpp"
#include "Age/LL/opengl.h"
#include "Age/Components/LightComponents.hpp"
#include "Age/UI/UI.hpp"
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	DeferredRenderer::DeferredRenderer()
	{
		debugPass = &egd.shaders.loadRaw(egd.res / "shader/deferredDebugger.rasl");
		postprocPass = &egd.shaders.loadPostproc(egd.res / "shader/deferredPostprocessing.pasl");
		dirLightPass = &egd.shaders.loadPostproc(egd.res / "shader/gbuffer/dirLight.pasl");
		shadowDirLightPass = &egd.shaders.loadPostproc(egd.res / "shader/gbuffer/shadowDirLight.pasl");
		pointLightPass = &egd.shaders.loadPostproc(egd.res / "shader/gbuffer/pointLight.pasl");
		spotLightPass = &egd.shaders.loadPostproc(egd.res / "shader/gbuffer/spotLight.pasl");
		iblPass = &egd.shaders.loadPostproc(egd.res / "shader/gbuffer/ibl.pasl");
		gbuffer.setTexturesCount(3);
	}

	void DeferredRenderer::clear()
	{
		gbuffer.removeRenderBuffer();
		albedoRoughnessMap.destroy();
		normalMetalnessMap.destroy();
		posMap.destroy();
		screenBuffer.destroy();
		depthBuffer.destroy();
	}

	void DeferredRenderer::updateSize(const uvec2& newSize)
	{
		size = newSize;
		ImageInfo baseColorRGB_RoughnessA{ newSize, TextureFormat::RGBA_F16 },
			normalRGB_MetalnessA{ newSize, TextureFormat::RGBA_F16 },
			posRGB{ newSize, TextureFormat::RGB_F32 },
			screenRGB{ newSize, TextureFormat::RGB_F16 },
			depthStencil{ newSize, TextureFormat::Depth24_Stencil8 };

		Sampler2DInfo sampler = { TextureFiltering::Near };
		albedoRoughnessMap.create(Texture2D::Settings{ baseColorRGB_RoughnessA, TextureFormat::AutoQuality, sampler, 1});
		normalMetalnessMap.create(Texture2D::Settings{ normalRGB_MetalnessA, TextureFormat::AutoQuality, sampler, 1});
		posMap.create(Texture2D::Settings{ posRGB, TextureFormat::AutoQuality, sampler, 1});
		screenBuffer.create(Texture2D::Settings{ screenRGB, TextureFormat::AutoQuality, sampler, 1});
		depthBuffer.create(Texture2D::Settings{ depthStencil, TextureFormat::AutoQuality, sampler, 1});

		ssao.create(newSize);
		gbuffer.setTexture(0, albedoRoughnessMap);
		gbuffer.setTexture(1, normalMetalnessMap);
		gbuffer.setTexture(2, posMap);
		gbuffer.createRenderBuffer(size);
		screenFb.setTexture(0, screenBuffer);
		screenFb.setDepthTexture(depthBuffer);
	}

	void DeferredRenderer::drawLightSources(const Node& node, const vec3& cameraPos)
	{
		node.forEachConst([&](const Node& n) {
			for (auto& comp : n.components)
			{
				const auto* const point = comp->as<PointLightComponent>(); 
				if (point)
				{
					const auto& l = point->getLight();
					pointLightPass->use();
					pointLightPass->setUniform(pointLightPass->getLocation("baseColor_roughness_map"), 0);
					pointLightPass->setUniform(pointLightPass->getLocation("normal_metalness_map"), 1);
					pointLightPass->setUniform(pointLightPass->getLocation("pos_map"), 2);
					pointLightPass->setUniform(pointLightPass->getLocation("ao_map"), 3);
					pointLightPass->setUniform(pointLightPass->getLocation("light.pos"), l.pos);
					pointLightPass->setUniform(pointLightPass->getLocation("light.ambient"), l.ambient * l.attOffset);
					pointLightPass->setUniform(pointLightPass->getLocation("light.color"), l.color * l.attOffset);
					pointLightPass->setUniform(pointLightPass->getLocation("light.sourceRadius"), l.size * 0.5f);
					pointLightPass->setUniform(pointLightPass->getLocation("light.attOffset"), l.attOffset);
					pointLightPass->setUniform(pointLightPass->getLocation("light.maxDist"), l.radius);
					pointLightPass->setUniform(pointLightPass->getLocation("cameraPos"), cameraPos);
					VertexBuffer::getDefFramebuf().draw();
					continue;
				}
				const auto* const spot = comp->as<SpotLightComponent>();
				if (spot)
				{
					const auto& l = spot->getLight();
					spotLightPass->use();
					spotLightPass->setUniform(spotLightPass->getLocation("baseColor_roughness_map"), 0);
					spotLightPass->setUniform(spotLightPass->getLocation("normal_metalness_map"), 1);
					spotLightPass->setUniform(spotLightPass->getLocation("pos_map"), 2);
					spotLightPass->setUniform(spotLightPass->getLocation("ao_map"), 3);
					spotLightPass->setUniform(spotLightPass->getLocation("light.pos"), l.pos);
					spotLightPass->setUniform(spotLightPass->getLocation("light.ambient"), l.ambient * l.attOffset);
					spotLightPass->setUniform(spotLightPass->getLocation("light.color"), l.color * l.attOffset);
					spotLightPass->setUniform(spotLightPass->getLocation("light.sourceRadius"), l.size * 0.5f);
					spotLightPass->setUniform(spotLightPass->getLocation("light.attOffset"), l.attOffset);
					spotLightPass->setUniform(spotLightPass->getLocation("light.maxDist"), l.radius);
					spotLightPass->setUniform(spotLightPass->getLocation("light.dir"), l.dir);
					spotLightPass->setUniform(spotLightPass->getLocation("light.cutOff"), cos(l.cutOff));
					spotLightPass->setUniform(spotLightPass->getLocation("light.outerCutOff"), cos(l.outerCutOff));
					spotLightPass->setUniform(spotLightPass->getLocation("cameraPos"), cameraPos);
					VertexBuffer::getDefFramebuf().draw();
					continue;
				}
				const auto* const dir = comp->as<DirLightComponent>(); 
				if (dir)
				{
					const auto& l = dir->getLight();
					if (l.useShadow)
					{
						shadowDirLightPass->use();
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("baseColor_roughness_map"), 0);
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("normal_metalness_map"), 1);
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("pos_map"), 2);
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("ao_map"), 3);
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("light.dir"), l.dir);
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("light.ambient"), l.ambient);
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("light.color"), l.color);
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("light.sourceRadius"), l.size * 0.5f);
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("light.shadowMap"), l.shadowMap, 4);
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("light.texelSize"), 
							vec2{1.f / l.shadowMap.getSize().x, 1.f / l.shadowMap.getSize().y});
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("light.bias"), dir->getBias());
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("shadowProj"), l.viewProj);
						shadowDirLightPass->setUniform(shadowDirLightPass->getLocation("cameraPos"), cameraPos);
					}
					else
					{
						dirLightPass->use();
						dirLightPass->setUniform(dirLightPass->getLocation("baseColor_roughness_map"), 0);
						dirLightPass->setUniform(dirLightPass->getLocation("normal_metalness_map"), 1);
						dirLightPass->setUniform(dirLightPass->getLocation("pos_map"), 2);
						dirLightPass->setUniform(dirLightPass->getLocation("ao_map"), 3);
						dirLightPass->setUniform(dirLightPass->getLocation("light.dir"), l.dir);
						dirLightPass->setUniform(dirLightPass->getLocation("light.ambient"), l.ambient);
						dirLightPass->setUniform(dirLightPass->getLocation("light.color"), l.color);
						dirLightPass->setUniform(dirLightPass->getLocation("light.sourceRadius"), l.size * 0.5f);
						dirLightPass->setUniform(dirLightPass->getLocation("cameraPos"), cameraPos);
					}
					VertexBuffer::getDefFramebuf().draw();
				}
			}
		});

		iblPass->use();
		iblPass->setUniform(iblPass->getLocation("baseColor_roughness_map"), 0);
		iblPass->setUniform(iblPass->getLocation("normal_metalness_map"), 1);
		iblPass->setUniform(iblPass->getLocation("pos_map"), 2);
		iblPass->setUniform(iblPass->getLocation("ao_map"), 3);
		iblPass->setUniform(iblPass->getLocation("diffuseMap"), 10);
		iblPass->setUniform(iblPass->getLocation("specularMap"), 11);
		iblPass->setUniform(iblPass->getLocation("brdfLut"), 12);
		iblPass->setUniform(iblPass->getLocation("maxSpecMipLevel"), float(TexEnums::getLastMipLevel(env->specular.getSize())));
		iblPass->setUniform(iblPass->getLocation("cameraPos"), cameraPos);
		VertexBuffer::getDefFramebuf().draw();
	}

	void DeferredRenderer::drawScene(const Scene& scene, const Camera& camera, float delta)
	{
		sf::Clock clock;
		//setup
		auto* rectangleVerts = &VertexBuffer::getDefFramebuf();

		SceneInfo deferredInfo, forwardInfo;
		deferredInfo.drawingCondition = [](const Material& m) {
			return m.shader->opaque &&
				not m.shader->requiresEmission &&
				not m.shader->customRendering;
			};
		ShaderSettings::Deferred deferred;
		deferred.implIndex = 0;
		deferredInfo.shaderSettings = deferred;
		forwardInfo.setCamera(camera);
		deferredInfo.setCamera(camera);
		forwardInfo.props.push_back({ "diffuseMap", 10 });
		forwardInfo.props.push_back({ "specularMap", 11 });
		forwardInfo.props.push_back({ "brdfLut", 12 });
		forwardInfo.props.push_back({ "maxSpecMipLevel", float(TexEnums::getLastMipLevel(env->specular.getSize())) });
		forwardInfo.addLights(*scene.rootNode);
		ShaderSettings::Forward forwardSettings;
		forwardSettings.pointLights = forwardInfo.lights.point;
		forwardSettings.dirLights = forwardInfo.lights.dir;
		forwardSettings.spotLights = forwardInfo.lights.spot;
		forwardSettings.dirLightsShadow = forwardInfo.lights.shadowDir;
		forwardInfo.shaderSettings = forwardSettings;

		Pipeline::setFrontFace();

		//gbuffer pass
		gbuffer.use();
		Pipeline::setBlendMode(BlendMode::Disable);
		Pipeline::set3DContext();
		Pipeline::enableStencil(true);
		Pipeline::setStencilWrite(0x1);
		Pipeline::clear({ 0.f, 0.f, 0.f });
		Pipeline::setStencilFunc(DepthFunc::Always, 0xFF);
		Pipeline::setStencilOp(StencilOp::Replace);
		scene.rootNode->forEachConst([&](const Node& n) {
			n.draw(deferredInfo);
		});

		gbufferTime = (int)clock.restart().asMicroseconds();

		forwardInfo.drawingCondition = [](const Material& m) {
			return m.shader->opaque && 
				(m.shader->requiresEmission || 
					m.shader->customRendering);
		};

		//ssao
		posMap.activate(2);
		normalMetalnessMap.activate(3);
		SSAO::getNoise().activate(4);
		ssao.use(2, 3, 4, camera.getProjection());

		//forward draw
		screenFb.use();
		Pipeline::clear({ 0.f, 0.f, 0.f });
		screenFb.copyFrom(gbuffer, FrameBuffer::Depth | FrameBuffer::Stencil);
		Pipeline::setStencilWrite(2);

		env->diffuse.activate(10);
		env->specular.activate(11);
		TextureLoader::getBrdfLut().activate(12);

		scene.rootNode->forEachConst([&](const Node& n) {
			n.draw(forwardInfo);
		});
		Pipeline::setStencilWrite(0);
		skyBox.draw(camera, nullptr);

		//draw lights
		Pipeline::set2DContext();
		Pipeline::setBlendMode(BlendMode::Add);
		Pipeline::setStencilFunc(DepthFunc::Equal, 1);
		albedoRoughnessMap.activate(0);
		normalMetalnessMap.activate(1);
		posMap.activate(2);
		ssao.getResult().activate(3);
		drawLightSources(*scene.rootNode, camera.transform.getPosition());

		//transparent objects
		Pipeline::setBlendMode(BlendMode::Lerp);
		Pipeline::set3DContext();
		Pipeline::enableStencil(false);
		forwardInfo.drawingCondition = [](const Material& m) {
			return !m.shader->opaque;
			};
		scene.rootNode->forEachConst([&](const Node& n) {
			n.draw(forwardInfo);
			});

		lightTime = (int)clock.restart().asMicroseconds();

		//draw on the screen
		Pipeline::set2DContext();
		bloom->useDownscale(screenBuffer);
		bloom->useUpscale();

		Pipeline::setBlendMode(BlendMode::Disable);

		const vec3 midColor = bloom->getTextures().crbegin()->getMidColor();
		const float brightness = vec3::dot(midColor, LUMA);
		const float clampedBr = Math::lerp(brightness, 0.5f, 0.3f);
		const float curExp = 0.25f / clampedBr;
		exposure = Math::smooth(exposure, curExp, delta * 3.f);

		FrameBuffer::useDefault(size);
		auto* verts = &VertexBuffer::getDefFramebuf();
		vec2 bloomRadius = {
			1.f / screenBuffer.getSize().x, 1.f / screenBuffer.getSize().y
		};
		postprocPass->use();
		postprocPass->setUniform(postprocPass->getLocation("tex"), screenBuffer, 0);
		postprocPass->setUniform(postprocPass->getLocation("bloomTex"), bloom->getTextures()[0], 1);
		postprocPass->setUniform(postprocPass->getLocation("exposure"), exposure);
		postprocPass->setUniform(postprocPass->getLocation("bloomStrength"), bloom->strength);
		postprocPass->setUniform(postprocPass->getLocation("bloomRadius"), bloomRadius * bloom->radius);
		postprocPass->setUniform(postprocPass->getLocation("fogColor"), fogColor);
		postprocPass->setUniform(postprocPass->getLocation("fogDist"), fogDistance);
		postprocPass->setUniform(postprocPass->getLocation("posMap"), posMap, 2);
		postprocPass->setUniform(postprocPass->getLocation("cameraPos"), camera.transform.getPosition());
		rectangleVerts->draw();

		if (debug)
		{
			debugPass->use();
			debugPass->setUniform(debugPass->getLocation("offset"), { 0.8f, 0.8f });
			debugPass->setUniform(debugPass->getLocation("scale"), { 0.2f, 0.2f });
			debugPass->setUniform(debugPass->getLocation("tex"), albedoRoughnessMap, 0);
			rectangleVerts->draw();
			debugPass->setUniform(debugPass->getLocation("offset"), { 0.8f, 0.4f });
			debugPass->setUniform(debugPass->getLocation("tex"), normalMetalnessMap, 1);
			rectangleVerts->draw();
			debugPass->setUniform(debugPass->getLocation("offset"), { 0.8f, 0.f });
			debugPass->setUniform(debugPass->getLocation("tex"), 2);
			rectangleVerts->draw();
		}

		UI::draw();

		screenTime = (int)clock.restart().asMicroseconds();
	}
}
