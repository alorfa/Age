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
		gbuffer.textures.resize(3);
		screenFb.textures.resize(1);
	}

	void DeferredRenderer::clear()
	{
		gbuffer.depthStencil.destroy();
		gbuffer.textures[0].destroy();
		gbuffer.textures[1].destroy();
		gbuffer.textures[2].destroy();
		screenFb.textures[0].destroy();
	}

	void DeferredRenderer::updateSize(const uvec2& newSize)
	{
		size = newSize;
		ImageInfo baseColorRGB_RoughnessA{ newSize, nullptr, TextureFormat::RGBA_Float16 },
			normalRGB_MetalnessA{ newSize, nullptr, TextureFormat::RGBA_Float16 },
			posRGB{ newSize, nullptr, TextureFormat::RGB_Float32 },
			screenRGB{ newSize, nullptr, TextureFormat::RGB_Float16 },
			depthStencil{ newSize, nullptr, TextureFormat::Depth24_Stencil8 };
		//gbuffer.createRenderBuffer(newSize);
		gbuffer.depthStencil.create({ depthStencil, false });
		gbuffer.textures[0].create({ baseColorRGB_RoughnessA, false });
		gbuffer.textures[1].create({ normalRGB_MetalnessA, false });
		gbuffer.textures[2].create({ posRGB, false });
		for (uint i = 0; i < gbuffer.textures.size(); i++)
		{
			gbuffer.textures[i].setFiltering(TextureFiltering::Near);
			gbuffer.textures[i].setWrap(TextureWrap::ClampToEdge);
		}
		gbuffer.create();

		screenFb.createRenderBuffer(newSize);
		screenFb.textures[0].create({ screenRGB , false });
		screenFb.textures[0].setFiltering(TextureFiltering::Near);
		screenFb.textures[0].setWrap(TextureWrap::ClampToEdge);
		screenFb.create();
	}

	void DeferredRenderer::drawObject(const Node& o, const SceneInfo& info)
	{
		o.forEachConst([&](const Node& n) {
			n.draw(info);
		});
	}

	void DeferredRenderer::drawLightSources(const Node& node, const vec3& cameraPos)
	{
		/*
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
					pointLightPass->setUniform(pointLightPass->getLocation("light.ambient"), l.ambient);
					pointLightPass->setUniform(pointLightPass->getLocation("light.color"), l.color);
					pointLightPass->setUniform(pointLightPass->getLocation("light.constant"), l.constant);
					pointLightPass->setUniform(pointLightPass->getLocation("light.linear"), l.linear);
					pointLightPass->setUniform(pointLightPass->getLocation("light.quadratic"), l.quadratic);
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
					spotLightPass->setUniform(spotLightPass->getLocation("light.ambient"), l.ambient);
					spotLightPass->setUniform(spotLightPass->getLocation("light.color"), l.color);
					spotLightPass->setUniform(spotLightPass->getLocation("light.constant"), l.constant);
					spotLightPass->setUniform(spotLightPass->getLocation("light.linear"), l.linear);
					spotLightPass->setUniform(spotLightPass->getLocation("light.quadratic"), l.quadratic);
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
					dirLightPass->setUniform(dirLightPass->getLocation("cameraPos"), cameraPos);
					VertexBuffer::getDefFramebuf().draw();
				}
			}
		});		
		*/
	}

	void DeferredRenderer::drawScene(const Scene& scene, const Camera& camera)
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
		drawObject(*scene.rootNode, deferredInfo);

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
		drawObject(*scene.rootNode, forwardInfo);
		Pipeline::setStencilWrite(0);
		scene.skyBox.draw(camera, nullptr);

		//draw lights
		Pipeline::set2DContext();
		Pipeline::setBlendMode(BlendMode::Add);
		Pipeline::setStencilFunc(DepthFunc::Equal, 1);
		gbuffer.textures[0].activate(0);
		gbuffer.textures[1].activate(1);
		gbuffer.textures[2].activate(2);
		drawLightSources(*scene.rootNode, camera.transform.getPosition());

		lightTime = (int)clock.restart().asMicroseconds();

		//transparent objects
		Pipeline::setBlendMode(BlendMode::Lerp);
		Pipeline::set3DContext();
		Pipeline::enableStencil(false);
		forwardInfo.drawingCondition = [](const Material& m) {
			return !m.shader->opaque;
			};
		drawObject(*scene.rootNode, forwardInfo);

		//draw on the screen
		Pipeline::setBlendMode(BlendMode::Disable);
		Pipeline::set2DContext();
		FrameBuffer::useDefault(size);
		postprocPass->use();
		postprocPass->setUniform(postprocPass->getLocation("tex"), screenFb.textures[0], 3);
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
