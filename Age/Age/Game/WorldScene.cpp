#include "WorldScene.hpp"
#include <SFML/Window/Event.hpp>
#include <Age/EventHandler.hpp>
#include "FollowToCamera.hpp"
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

namespace a_game
{
	WorldScene::WorldScene()
	{
		rootNode->addComponent<PlayerController>();
		rootNode->addComponent<SceneController>();
		deferredRenderer.bloom = forwardRenderer.bloom = &bloom;
	}
	void WorldScene::load()
	{
		activeCamera = &egd.camera;

		std::unique_ptr<Node> objs[6];
		for (uint i = 0; i < 6; i++)
			objs[i] = std::make_unique<Node>();
		MeshComponent::addModel(*objs[0], egd.models.load(egd.res / "model/daedric/scene.gltf",
			ModelLoader::Settings{ vec3{5.f}, false, false, true }));
		MeshComponent::addModel(*objs[1], egd.models.load(egd.res / "model/kirara/scene.gltf",
			ModelLoader::Settings{ vec3{10.f}, false}));
		MeshComponent::addModel(*objs[3], egd.models.load(egd.res / "model/sphere.obj"));
		MeshComponent::addModel(*objs[4], egd.models.load(egd.res / "model/sphere.obj"));
		MeshComponent::setShader(*objs[0], egd.shaders.load(egd.res / "shader/pbrNormal.asl"));
		MeshComponent::setShader(*objs[1], egd.shaders.load(egd.res / "shader/kirara.asl"));
		Shader& lightShader = egd.shaders.load(egd.res / "shader/lightSource.asl");
		lightShader.requiresEmission = true;
		MeshComponent::setShader(*objs[3], lightShader);
		MeshComponent::setShader(*objs[4], lightShader); 
		//objs[0]->addComponent(std::make_unique<Rotate>(*objs[0]));

		const bool one_light_test = false;
		const bool indirect_light_test = false;

		vec2 floorPositions[3][3] = {
			{vec2{-10.f, -10.f}, vec2{-10.f, 0.f}, vec2{-10.f, 10.f}},
			{vec2{0.f, -10.f}, vec2{0.f, 0.f}, vec2{0.f, 10.f}},
			{vec2{10.f, -10.f}, vec2{10.f, 0.f}, vec2{10.f, 10.f}}
		};
		for (uint i = 0; i < 3; i++)
			for (uint j = 0; j < 3; j++)
			{
				auto& floor = rootNode->addChild();
				MeshComponent::addModel(floor, egd.models.load(egd.res / "model/10m.obj", ModelLoader::Settings{
					vec3{1.f}, true, false, false, false }));
				MeshComponent::setShader(floor, egd.shaders.load(egd.res / "shader/floor.asl"));
				floor.setPosition({ floorPositions[i][j].x, floorPositions[i][j].y, 0.f});
			}

		objs[2]->addComponent<FollowToCamera>()
			.setCamera(*activeCamera);
		/*objs[4]->addComponent<RotateComp>()
			.init(vec3(-2.f, 1.5f, -1.5f), vec3(-3.f, 3.f, 0.f), 0.5f);*/

		if (!indirect_light_test)
		{
			objs[4]->addComponent<PointLightComponent>()
				.setSize(0.2f)
				.setColor({ 5.0f, 0.5f, 0.5f }, 0.03f)
				.addModel(*objs[4]);

			if (one_light_test)
			{
				objs[3]->forEach([](Node& n) {
					auto meshes = n.findAllComponents<MeshComponent>();
					for (auto m : meshes)
						m->mesh.material.setValue("emission", ShaderProperty(vec3{ 0.f }));
					});
			}
			else
			{
				objs[2]->addComponent<SpotLightComponent>()
					.setColor({ 4.f, 4.f, 10.f }, 0.03f)
					.setSize(0.1f)
					.setCutOff(0.f)
					.setOuterCutOff(Math::rad(15.f));
				objs[3]->addComponent<PointLightComponent>()
					.setColor({ 8.f, 6.f, 2.f }, 0.03f)
					.addModel(*objs[3])
					.setSize(0.2f);
				objs[5]->addComponent<DirLightComponent>()
					.setSize(0.1f)
					.setDirection({ 0.f, -0.5f, 1.f });
			}
		}

		objs[0]->changeTransform().changePosition() = { -3, 5, 1 };
		objs[1]->changeTransform().changePosition() = { 0, 5, 0 };
		objs[3]->changeTransform().changePosition() = vec3(-1.f, 5, 2);
		objs[3]->changeTransform().changeScale() *= 0.1f;
		objs[4]->changeTransform().changePosition() = vec3(-3.f, 4, 0.1f);
		objs[4]->changeTransform().changeScale() *= 0.1f;

		CubeMapLoader::RawSettings s;
		s.upperLimit = 20000.f;
		SkyBox::cube = &egd.models.load(egd.res / "model/skybox.obj").meshes[0].get()->buffer;
		deferredRenderer.skyBox.shader = forwardRenderer.skyBox.shader = 
			&egd.shaders.loadRaw(egd.res / "shader/skyboxMip0.rasl");
		env = &egd.cubemaps.load(egd.res / "img/skybox1k.exr");
		deferredRenderer.skyBox.cubemap = forwardRenderer.skyBox.cubemap = 
			&egd.cubemaps.loadRaw(egd.res / "img/skybox4k.exr", s);
		deferredRenderer.env = forwardRenderer.env = env;

		for (uint i = 0; i < 11; i++)
			for (uint j = 0; j < 11; j++)
			{
				auto& sphere = rootNode->addChild();
				MeshComponent::addModel(sphere, egd.models.load(egd.res / "model/sphere.obj",
					ModelLoader::Settings{vec3{1.f}, false}));
				MeshComponent::setShader(sphere, egd.shaders.load(egd.res / "shader/testSus.asl"));
				sphere.forEach([&](Node& n) {
					auto meshes = n.findAllComponents<MeshComponent>();
					for (auto& mesh : meshes)
					{
						mesh->mesh.material.setValue("roughness", ShaderProperty((float)i * 0.1f));
						mesh->mesh.material.setValue("metallic", ShaderProperty((float)j * 0.1f));
					}
				});
				sphere.setPosition({ (float)i * 0.4f - 5.f, (float)j * 0.4f - 1.5f, 0.25f });
				sphere.setScale({ 0.25f });
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
				sphere.setPosition({ (float)i - 4.f, (float)j - 0.5f, 0.8f});
				sphere.setScale({ 0.3f });
				sphere.forEach([&](Node& n) {
					auto meshes = n.findAllComponents<MeshComponent>();
					for (auto& mesh : meshes)
						mesh->mesh.material.setValue("color", ShaderProperty(colors[i][j]));
				});
			}

		for (uint i = 0; i < 6; i++)
			rootNode->addChild(std::move(objs[i]));
	}
	void WorldScene::draw(const Camera* c, float delta)
	{
		setActiveRenderer(rendererIndex);
		bloom.upscaleStartMipLevel = bloomMipCount - 1;
		bloom.blendMode = bloomFogBlending ? BlendMode::Lerp : BlendMode::Add;
		const Camera* camera = c ? c : activeCamera;

		rootNode->sortChildren(camera->transform.getPosition(), Node::Transparent);
		activeRenderer->drawScene(*this, *camera, delta);
		if (activeRenderer == &deferredRenderer)
			egd.window->setTitle(std::format("Gbuffer: {}, Light: {}, Screen: {}",
				deferredRenderer.gbufferTime, deferredRenderer.lightTime, deferredRenderer.screenTime));
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
