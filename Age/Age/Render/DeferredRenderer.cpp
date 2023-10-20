#include "DeferredRenderer.hpp"
#include "Age/egd.hpp"
#include "Age/Object/Node3D.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Scene/SkyBox.hpp"
#include "Age/Scene/Scene3D.hpp"
#include "Age/Light/LightSource.hpp"

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

	void DeferredRenderer::drawObject(const Node3D& o, const Scene3DInfo& info)
	{
		o.draw(info);
		o.forEachConst([&](const Node3D& n)
			{
				n.draw(info);
			});
	}

	void DeferredRenderer::drawLightSources(const Node3D& node, const vec3& cameraPos)
	{
		const DirLightSource* dir = node.as<DirLightSource>();
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
		const PointLightSource* point = node.as<PointLightSource>();
		if (point)
		{
			pointLightPass->use();
			pointLightPass->setUniform(pointLightPass->getLocation("baseColor_roughness_map"), 0);
			pointLightPass->setUniform(pointLightPass->getLocation("normal_metalness_map"), 1);
			pointLightPass->setUniform(pointLightPass->getLocation("pos_map"), 2);
			pointLightPass->setUniform(pointLightPass->getLocation("light.pos"), point->light.pos);
			pointLightPass->setUniform(pointLightPass->getLocation("light.ambient"), point->light.ambient);
			pointLightPass->setUniform(pointLightPass->getLocation("light.color"), point->light.color);
			pointLightPass->setUniform(pointLightPass->getLocation("light.constant"), point->light.constant);
			pointLightPass->setUniform(pointLightPass->getLocation("light.linear"), point->light.linear);
			pointLightPass->setUniform(pointLightPass->getLocation("light.quadratic"), point->light.quadratic);
			pointLightPass->setUniform(pointLightPass->getLocation("cameraPos"), cameraPos);
			VertexBuffer::getDefFramebuf().draw();
		}
		const SpotLightSource* spot = node.as<SpotLightSource>();
		if (spot)
		{
			spotLightPass->use();
			spotLightPass->setUniform(spotLightPass->getLocation("baseColor_roughness_map"), 0);
			spotLightPass->setUniform(spotLightPass->getLocation("normal_metalness_map"), 1);
			spotLightPass->setUniform(spotLightPass->getLocation("pos_map"), 2);
			spotLightPass->setUniform(spotLightPass->getLocation("light.pos"), spot->light.pos);
			spotLightPass->setUniform(spotLightPass->getLocation("light.ambient"), spot->light.ambient);
			spotLightPass->setUniform(spotLightPass->getLocation("light.color"), spot->light.color);
			spotLightPass->setUniform(spotLightPass->getLocation("light.constant"), spot->light.constant);
			spotLightPass->setUniform(spotLightPass->getLocation("light.linear"), spot->light.linear);
			spotLightPass->setUniform(spotLightPass->getLocation("light.quadratic"), spot->light.quadratic);
			spotLightPass->setUniform(spotLightPass->getLocation("light.dir"), spot->light.dir);
			spotLightPass->setUniform(spotLightPass->getLocation("light.cutOff"), cos(spot->light.cutOff));
			spotLightPass->setUniform(spotLightPass->getLocation("light.outerCutOff"), cos(spot->light.outerCutOff));
			spotLightPass->setUniform(spotLightPass->getLocation("cameraPos"), cameraPos);
			VertexBuffer::getDefFramebuf().draw();
		}
		node.forEachConst([&](const Node3D& n)
			{
				drawLightSources(n, cameraPos);
			});
		
	}

	void DeferredRenderer::drawScene(const Scene3D& scene, const Camera3D& camera)
	{
		sf::Clock clock;
		//setup
		auto* rectangleVerts = &VertexBuffer::getDefFramebuf();

		Scene3DInfo info;
		info.camera = &camera;
		ShaderSettings::Deferred deferred;
		deferred.bindings = { 4, 4, 3 };
		deferred.paintingFuncIndex = 0;
		info.shaderSettings = deferred;

		//gbuffer pass
		gbuffer.use();
		Pipeline::setBlendMode(BlendMode::Disable);
		Pipeline::set3DContext();
		Pipeline::clear({ 0.f, 0.f, 0.f });
		drawObject(*scene.rootNode, info);

		gbufferTime = clock.restart().asMicroseconds();

		//draw lights
		screenFb.use();
		Pipeline::clear({ 0.f, 0.f, 0.f });
		screenFb.copyFrom(gbuffer, FrameBuffer::Depth);
		scene.skyBox.draw(camera, nullptr);
		Pipeline::set2DContext();
		Pipeline::setBlendMode(BlendMode::Add);
		gbuffer.textures[0].activate(0);
		gbuffer.textures[1].activate(1);
		gbuffer.textures[2].activate(2);
		drawLightSources(*scene.rootNode, camera.transform.getPosition());

		lightTime = clock.restart().asMicroseconds();

		//draw on the screen
		Pipeline::setBlendMode(BlendMode::Disable);
		FrameBuffer::useDefault(size);
		postprocPass->use();
		postprocPass->setUniform(postprocPass->getLocation("tex"), screenFb.textures[0], 3);
		rectangleVerts->draw();

		//debug
		debugPass->use();
		debugPass->setUniform(debugPass->getLocation("offset"), {0.8f, 0.8f});
		debugPass->setUniform(debugPass->getLocation("scale"), { 0.2f, 0.2f });
		debugPass->setUniform(debugPass->getLocation("tex"), 0);
		rectangleVerts->draw();
		debugPass->setUniform(debugPass->getLocation("offset"), { 0.8f, 0.4f });
		debugPass->setUniform(debugPass->getLocation("tex"), 1);
		rectangleVerts->draw();
		debugPass->setUniform(debugPass->getLocation("offset"), { 0.8f, 0.f });
		debugPass->setUniform(debugPass->getLocation("tex"), 2);
		rectangleVerts->draw();

		screenTime = clock.restart().asMicroseconds();
	}
}
