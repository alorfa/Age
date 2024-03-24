#include "WorldScene.hpp"
#include <SFML/Window/Event.hpp>
#include <Age/EventHandler.hpp>
#include "FlashLight.hpp"
#include "Rotate.hpp"
#include "Age/egd.hpp"
#include "Age/Material/Shader.hpp"
#include "Age/Resource/File.hpp"
#include <Age/Components/MeshComponent.hpp>
#include <Age/Components/LightComponents.hpp>
#include "Age/Object/Node.hpp"
#include "SceneController.hpp"
#include "PlayerController.hpp"
#include "RotateComp.hpp"
#include "Age/Math/Math.hpp"
#include "SunComp.hpp"
#include "imgui.h"

namespace a_game
{
	namespace
	{
		void combineTextures(
			const std::filesystem::path& rgb,
			const std::filesystem::path& alpha,
			const std::filesystem::path& result)
		{

			Image imgRgb, imgAlpha, imgResult;
			imgRgb.loadFromFile(rgb);
			imgAlpha.loadFromFile(alpha);
			imgResult.info.format = TextureFormat::RGBA;
			imgResult.info.size = imgRgb.info.size;

			Texture2D texRgb, texAlpha, texResult;
			texRgb.create(Texture2D::Settings(imgRgb.info, TextureFormat::Auto));
			texAlpha.create(Texture2D::Settings(imgAlpha.info, TextureFormat::Auto));
			texResult.create(Texture2D::Settings(imgResult.info, TextureFormat::Auto));

			FrameBuffer2D fb;
			fb.setTexture(0, texResult);
			const auto& shader = egd.shaders.loadPostproc(egd.res / "img/combine.pasl");
			fb.use();
			shader.use();
			shader.setUniform(shader.getLocation("texRgb"), texRgb, 0);
			shader.setUniform(shader.getLocation("texAlpha"), texAlpha, 1);

			VertexBuffer::getDefFramebuf().draw();

			imgResult.createFromTexture(texResult, TextureFormat::RGBA);
			imgResult.info.flipVertically();
			imgResult.saveToFile(result);
		}
	}

	WorldScene::WorldScene()
	{
		rootNode->addComponent<PlayerController>();
		rootNode->addComponent<SceneController>();
		deferredRenderer.bloom = forwardRenderer.bloom = &bloom;
	}
	void WorldScene::load()
	{
		activeCamera = &egd.camera;

		auto& sun = rootNode->addChild();

		auto& daedric = rootNode->addChild();
		MeshComponent::addModel(daedric, egd.models.load(egd.res / "model/daedric/scene.gltf",
			ModelLoader::Settings{ vec3{5.f}, false, false, true }));
		MeshComponent::setShader(daedric, egd.shaders.load(egd.res / "shader/pbrNormal.asl"));
		daedric.changeTransform().changePosition() = { -2, 5, 1 };
		daedric.changeTransform().changeRotation().z = Math::rad(90.f);

		auto& kirara = rootNode->addChild();
		MeshComponent::addModel(kirara, egd.models.load(egd.res / "model/kirara/scene.gltf",
			ModelLoader::Settings{ vec3{10.f}, false}));
		MeshComponent::setShader(kirara, egd.shaders.load(egd.res / "shader/kirara.asl"));
		kirara.changeTransform().changePosition() = { 0, 5, 0 };

		auto& light1 = rootNode->addChild();
		auto& light2 = rootNode->addChild();
		auto& light3 = rootNode->addChild();

		MeshComponent::addModel(light1, egd.models.load(egd.res / "model/sphere.obj"));
		MeshComponent::addModel(light2, egd.models.load(egd.res / "model/sphere.obj"));
		MeshComponent::addModel(light3, egd.models.load(egd.res / "model/sphere.obj"));
		Shader& lightShader = egd.shaders.load(egd.res / "shader/lightSource.asl");
		lightShader.requiresEmission = true;
		MeshComponent::setShader(light1, lightShader);
		MeshComponent::setShader(light2, lightShader);
		MeshComponent::setShader(light3, lightShader);

		const bool one_light_test = false;
		const bool indirect_light_test = false;

		auto& floor = rootNode->addChild();
		MeshComponent::addModel(floor, egd.models.load(egd.res / "model/plane.obj", ModelLoader::Settings{
			vec3{1.f}, true, false, false, false }));
		MeshComponent::setShader(floor, egd.shaders.load(egd.res / "shader/grass.asl"));
		floor.forEach([](Node& n) {
				Sampler2DInfo samplerRepeatMip = { TextureFiltering::Linear_MipLinear, TextureWrap::Repeat };
				auto meshes = n.findAllComponents<MeshComponent>();
				ShaderProperty::Texture2DProp albedoProp = { egd.textures.load(egd.res / "img/grassAlbedoRoughness.png",
					TextureLoader::Settings{samplerRepeatMip, TextureFormat::S_RGBA}), 0 };
				ShaderProperty::Texture2DProp normalProp = { egd.textures.load(egd.res / "img/grassNormalAo.png",
					TextureLoader::Settings{samplerRepeatMip, TextureFormat::RGBA_8}), 1 };
				for (auto& m : meshes) {
					m->mesh.material.setValue("baseColorMap", ShaderProperty(albedoProp));
					m->mesh.material.setValue("normalMap", ShaderProperty(normalProp));
				}
			});
		floor.setPosition({ 0.f });
			

		auto& flashlight = rootNode->addChild();
		flashlight.addComponent<FlashLight>()
			.setCamera(*activeCamera);
		/*objs[4]->addComponent<RotateComp>()
			.init(vec3(-2.f, 1.5f, -1.5f), vec3(-3.f, 3.f, 0.f), 0.5f);*/

		if (!indirect_light_test)
		{
			light2.addComponent<PointLightComponent>()
				.setSize(0.2f)
				.setColor({ 5.0f, 0.5f, 0.5f }, 0.03f)
				.addModel(light2);

			if (one_light_test)
			{
				light1.forEach([](Node& n) {
					auto meshes = n.findAllComponents<MeshComponent>();
					for (auto m : meshes)
						m->mesh.material.setValue("emission", ShaderProperty(vec3{ 0.f }));
					});
			}
			else
			{
				flashlight.addComponent<SpotLightComponent>()
					.setColor({ 4.f, 4.f, 8.f }, 0.03f)
					.setSize(0.1f)
					.setCutOff(0.f)
					.setOuterCutOff(Math::rad(15.f));
				light1.addComponent<PointLightComponent>()
					.setColor({ 8.f, 6.f, 2.f }, 0.03f)
					.addModel(light1)
					.setSize(0.2f);
				light3.addComponent<PointLightComponent>()
					.setColor({ 5.f, 1.5f, 7.f }, 0.03f)
					.addModel(light3)
					.setSize(0.2f);
				sun.addComponent<DirLightComponent>()
					.setSize(0.08f)
					.setDirection({ 0.f, 0.5f, 0.51f })
					.createShadowMap({2048u}, TextureFormat::Depth16)
					.setBias(0.0002f)
					;
				sun.addComponent<SunComp>();
				sun.setPosition({ -1.f, 4.f, 1.f });
			}
		}

		light1.changeTransform().changePosition() = vec3(-1.f, 5, 2);
		light1.changeTransform().changeScale() *= 0.1f;
		light2.changeTransform().changePosition() = vec3(-3.f, 4, 0.1f);
		light2.changeTransform().changeScale() *= 0.1f;
		light3.changeTransform().changeScale() *= 0.1f;
		light3.addComponent<RotateComp>()
			.init({-3.f, 1.f, 1.5f}, {-1.5f, 1.5f, 2.f}, 0.3f);

		CubeMapLoader::RawSettings s;
		s.upperLimit = 20000.f;
		SkyBox::cube = &egd.models.load(egd.res / "model/skybox.obj").meshes[0].get()->buffer;
		deferredRenderer.skyBox.shader = forwardRenderer.skyBox.shader = 
			&egd.shaders.loadRaw(egd.res / "shader/skyboxMip0.rasl");
		env = &egd.cubemaps.load(egd.res / "img/skybox1k.exr");
		deferredRenderer.skyBox.cubemap = forwardRenderer.skyBox.cubemap = 
			&egd.cubemaps.loadRaw(egd.res / "img/skybox4k.exr", s);
		deferredRenderer.env = forwardRenderer.env = env;

		for (uint i = 0; i < 6; i++)
			for (uint j = 0; j < 6; j++)
			{
				auto& sphere = rootNode->addChild();
				MeshComponent::addModel(sphere, egd.models.load(egd.res / "model/sphere.obj",
					ModelLoader::Settings{vec3{1.f}, false}));
				MeshComponent::setShader(sphere, egd.shaders.load(egd.res / "shader/testSus.asl"));
				sphere.forEach([&](Node& n) {
					auto meshes = n.findAllComponents<MeshComponent>();
					for (auto& mesh : meshes)
					{
						mesh->mesh.material.setValue("roughness", ShaderProperty(1.f - (float)i * 0.2f));
						mesh->mesh.material.setValue("metallic", ShaderProperty((float)j * 0.2f));
					}
				});
				sphere.setPosition({ (float)i * 0.8f - 5.f, (float)j * 0.8f - 1.5f, 0.35f });
				sphere.setScale({ 0.35f });
			}
		vec3 colors[3][3] = {
			vec3{1.f, 0.1f, 0.1f}, vec3{0.1f, 1.f, 0.1f}, vec3{0.1f, 0.1f, 1.f},
			vec3{1.f, 1.f, 0.1f}, vec3{0.1f, 1.f, 1.f}, vec3{1.f, 0.1f, 1.f},
			vec3{1.f, 1.f, 1.f}, vec3{0.5f, 0.5f, 0.5f}, vec3{0.1f, 0.1f, 0.1f}
		};
		egd.shaders.load(egd.res / "shader/glass.asl").opaque = false;
		for (uint i = 0; i < 3; i++)
			for (uint j = 0; j < 3; j++)
			{
				auto& sphere = rootNode->addChild(Node::Transparent);
				MeshComponent::addModel(sphere, egd.models.load(egd.res / "model/sphere.obj",
					ModelLoader::Settings{ vec3{1.f}, false }));
				MeshComponent::setShader(sphere, egd.shaders.load(egd.res / "shader/glass.asl"));
				sphere.setPosition({ (float)i - 4.f, (float)j - 0.5f, 1.f});
				sphere.setScale({ 0.3f });
				sphere.forEach([&](Node& n) {
					auto meshes = n.findAllComponents<MeshComponent>();
					for (auto& mesh : meshes)
						mesh->mesh.material.setValue("color", ShaderProperty(colors[i][j]));
				});
			}
	}
	void WorldScene::draw(const Camera* c, float delta)
	{
		setActiveRenderer(rendererIndex);
		bloom.upscaleStartMipLevel = bloomMipCount - 1;
		bloom.blendMode = bloomFogBlending ? BlendMode::Lerp : BlendMode::Add;
		deferredRenderer.fogColor = fogColor.pow(2.2f);
		deferredRenderer.fogDistance = fogDistance;
		deferredRenderer.disableAo = enableSSAO ? 0.f : 1.f;
		const Camera* camera = c ? c : activeCamera;

		rootNode->sortChildren(camera->transform.getPosition(), Node::Transparent);
		activeRenderer->drawScene(*this, *camera, delta);
		if (false)
		//if (activeRenderer == &deferredRenderer)
			egd.window->setTitle(std::format("Gbuffer: {}, Light: {}, Screen: {}",
				deferredRenderer.gbufferTime, deferredRenderer.lightTime, deferredRenderer.screenTime));
		else
		{
			egd.window->setTitle(std::format("A graphics engine (OpenGL 4.6). Fps: {}", ImGui::GetIO().Framerate));
		}
	}
	void WorldScene::updateSize(uvec2 size)
	{
		activeRenderer->updateSize(size);
		bloom.create(size / 2u, 0);
	}
	void WorldScene::setActiveRenderer(int index)
	{
		Renderer* curRender = nullptr;
		if (index == 0)
			curRender = &forwardRenderer;
		if (index == 1)
			curRender = &deferredRenderer;

		if (curRender == nullptr or curRender == activeRenderer)
			return;

		uvec2 size = { egd.window->getSize().x, egd.window->getSize().y };
		if (activeRenderer)
			activeRenderer->clear();
		activeRenderer = curRender;
		activeRenderer->updateSize(size);
		bloom.create(size / 2u, 0);
	}
}
