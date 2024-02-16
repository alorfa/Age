#include "DeferredRenderer.hpp"
#include "Age/egd.hpp"
#include "Age/Object/Node.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Scene/SkyBox.hpp"
#include "Age/Scene/Scene.hpp"
#include "Age/LL/opengl.h"
#include "Age/Components/LightComponents.hpp"

namespace a_game_engine
{
	DeferredRenderer::DeferredRenderer()
	{
		debugPass = &egd.shaders.loadRaw(egd.res / "shader/deferredDebugger.rasl");
		postprocPass = &egd.shaders.loadPostproc(egd.res / "shader/postproc.pasl");
		dirLightPass = &egd.shaders.loadPostproc(egd.res / "shader/dirLight.pasl");
		pointLightPass = &egd.shaders.loadPostproc(egd.res / "shader/pointLight.pasl");
		spotLightPass = &egd.shaders.loadPostproc(egd.res / "shader/spotLight.pasl");
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
		ImageInfo baseColorRGB_RoughnessA{ newSize, TextureFormat::RGBA_Float16 },
			normalRGB_MetalnessA{ newSize, TextureFormat::RGBA_Float16 },
			posRGB{ newSize, TextureFormat::RGB_Float32 },
			screenRGB{ newSize, TextureFormat::RGB_Float16 },
			depthStencil{ newSize, TextureFormat::Depth24_Stencil8 };

		Sampler2DInfo sampler = { TextureWrap::ClampToEdge, TextureFiltering::Near };
		albedoRoughnessMap.create(Texture2D::Settings{ baseColorRGB_RoughnessA, TextureFormat::Auto, sampler, 1});
		normalMetalnessMap.create(Texture2D::Settings{ normalRGB_MetalnessA, TextureFormat::Auto, sampler, 1});
		posMap.create(Texture2D::Settings{ posRGB, TextureFormat::Auto, sampler, 1});
		screenBuffer.create(Texture2D::Settings{ screenRGB, TextureFormat::Auto, sampler, 1});
		depthBuffer.create(Texture2D::Settings{ depthStencil, TextureFormat::Auto, sampler, 1});

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
					dirLightPass->use();
					dirLightPass->setUniform(dirLightPass->getLocation("baseColor_roughness_map"), 0);
					dirLightPass->setUniform(dirLightPass->getLocation("normal_metalness_map"), 1);
					dirLightPass->setUniform(dirLightPass->getLocation("pos_map"), 2);
					dirLightPass->setUniform(dirLightPass->getLocation("light.dir"), dir->light.dir);
					dirLightPass->setUniform(dirLightPass->getLocation("light.ambient"), dir->light.ambient);
					dirLightPass->setUniform(dirLightPass->getLocation("light.color"), dir->light.color);
					dirLightPass->setUniform(dirLightPass->getLocation("light.sourceRadius"), dir->light.size * 0.5f);
					dirLightPass->setUniform(dirLightPass->getLocation("cameraPos"), cameraPos);
					VertexBuffer::getDefFramebuf().draw();
				}
			}
		});		
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
		forwardInfo.camera = deferredInfo.camera = &camera;
		forwardInfo.props.push_back({ "skybox", SkyBox::getSlot() });
		forwardInfo.addLights(*scene.rootNode);
		ShaderSettings::Forward forwardSettings;
		forwardSettings.pointLights = forwardInfo.lights.point;
		forwardSettings.dirLights = forwardInfo.lights.dir;
		forwardSettings.spotLights = forwardInfo.lights.spot;
		forwardInfo.shaderSettings = forwardSettings;

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
		//forward draw
		screenFb.use();
		Pipeline::clear({ 0.f, 0.f, 0.f });
		screenFb.copyFrom(gbuffer, FrameBuffer::Depth | FrameBuffer::Stencil);
		Pipeline::setStencilWrite(2);
		scene.rootNode->forEachConst([&](const Node& n) {
			n.draw(forwardInfo);
		});
		Pipeline::setStencilWrite(0);
		scene.skyBox.draw(camera, nullptr);

		//draw lights
		Pipeline::set2DContext();
		Pipeline::setBlendMode(BlendMode::Add);
		Pipeline::setStencilFunc(DepthFunc::Equal, 1);
		albedoRoughnessMap.activate(0);
		normalMetalnessMap.activate(1);
		posMap.activate(2);
		drawLightSources(*scene.rootNode, camera.transform.getPosition());

		lightTime = (int)clock.restart().asMicroseconds();

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

		//draw on the screen
		Pipeline::setBlendMode(BlendMode::Disable);
		Pipeline::set2DContext();
		FrameBuffer::useDefault(size);
		postprocPass->use();
		postprocPass->setUniform(postprocPass->getLocation("tex"), screenBuffer, 3);
		rectangleVerts->draw();

		if (debug)
		{
			debugPass->use();
			debugPass->setUniform(debugPass->getLocation("offset"), { 0.8f, 0.8f });
			debugPass->setUniform(debugPass->getLocation("scale"), { 0.2f, 0.2f });
			debugPass->setUniform(debugPass->getLocation("tex"), 0);
			rectangleVerts->draw();
			debugPass->setUniform(debugPass->getLocation("offset"), { 0.8f, 0.4f });
			debugPass->setUniform(debugPass->getLocation("tex"), 1);
			rectangleVerts->draw();
			debugPass->setUniform(debugPass->getLocation("offset"), { 0.8f, 0.f });
			debugPass->setUniform(debugPass->getLocation("tex"), 2);
			rectangleVerts->draw();
		}

		screenTime = (int)clock.restart().asMicroseconds();
	}
}
