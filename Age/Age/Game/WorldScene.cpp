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
#include "ControlController.hpp"
#include "RotateComp.hpp"

namespace a_game
{
	WorldScene::WorldScene()
	{
		rootNode->addComponent<ControlController>();
		rootNode->addComponent<SceneController>();
	}
	void WorldScene::load()
	{
		auto nativeSize = egd.window->getSize();
		forwardRenderer.updateSize({ nativeSize.x, nativeSize.y });
		activeRenderer = &forwardRenderer;

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
		MeshComponent::setShader(*objs[1], egd.shaders.load(egd.res / "shader/default.asl"));
		Shader& lightShader = egd.shaders.load(egd.res / "shader/lightSource.asl");
		lightShader.requiresEmission = true;
		MeshComponent::setShader(*objs[3], lightShader);
		MeshComponent::setShader(*objs[4], lightShader);
		//objs[0]->addComponent(std::make_unique<Rotate>(*objs[0]));

		const bool light_test = false;

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
				floor.setPosition({ floorPositions[i][j].x, floorPositions[i][j].y, -4.f});
			}

		objs[2]->addComponent<FollowToCamera>()
			.setCamera(*activeCamera);
		objs[4]->addComponent<PointLightComponent>()
			.setSize(0.2f)
			.setOffset(2.f)
			.setColor({ 3.0f, 0.3f, 0.3f }, 0.03f)
			.addModel(*objs[4]);
		/*objs[4]->addComponent<RotateComp>()
			.init(vec3(-2.f, 1.5f, -3.5f), vec3(-3.f, 3.f, 0.f), 0.5f);*/

		if (light_test)
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
				.setColor({ 3.f, 3.f, 8.f }, 0.03f)
				.setOffset(2.f)
				.setSize(0.1f);
			objs[3]->addComponent<PointLightComponent>()
				.setColor({ 6.f, 4.f, 1.5f }, 0.03f)
				.setOffset(2.f)
				.addModel(*objs[3])
				.setSize(0.2f);
			objs[5]->addComponent<DirLightComponent>()
				.setSize(0.1f)
				.setDirection({0.f, -0.5f, 1.f});
		}

		objs[0]->changeTransform().changePosition() = { -3, 5, 0 };
		objs[1]->changeTransform().changePosition() = { 0, 5, -1 };
		objs[3]->changeTransform().changePosition() = vec3(-1.f, 5, 2);
		objs[3]->changeTransform().changeScale() *= 0.1f;
		objs[4]->changeTransform().changePosition() = vec3(-3.f, 4, -2);
		objs[4]->changeTransform().changeScale() *= 0.1f;

		std::filesystem::path cubePaths[6];
		cubePaths[0] = egd.res / "img/sky/+x.jpg";
		cubePaths[1] = egd.res / "img/sky/-x.jpg";
		cubePaths[2] = egd.res / "img/sky/+y.jpg";
		cubePaths[3] = egd.res / "img/sky/-y.jpg";
		cubePaths[4] = egd.res / "img/sky/+z.jpg";
		cubePaths[5] = egd.res / "img/sky/-z.jpg";
		SkyBox::cube = &egd.models.load(egd.res / "model/skybox.obj").meshes[0].get()->buffer;
		skyBox.cubemap = &egd.textures.loadCubeMap(cubePaths, TextureLoader::CubemapSettings{
			TextureFiltering::LinearMipLinear, TextureFiltering::Linear});
		skyBox.shader = &egd.shaders.loadRaw(egd.res / "shader/skybox.rasl");

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
						mesh->mesh.material.setValue("metalic", ShaderProperty((float)i * 0.1f));
						mesh->mesh.material.setValue("roughness", ShaderProperty((float)j * 0.1f));
					}
				});
				sphere.setPosition({ (float)i * 0.4f - 5.f, (float)j * 0.4f, -3.f });
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
				sphere.setPosition({ (float)i - 4.f, (float)j + 1.f, -2.3f});
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
	void WorldScene::draw(const Camera* c, float delta) const
	{
		const Camera* camera = c ? c : activeCamera;
		rootNode->sortChildren(camera->transform.getPosition(), Node::Transparent);
		activeRenderer->drawScene(*this, *camera, delta);
		if (activeRenderer == &deferredRenderer)
			egd.window->setTitle(std::format("Gbuffer: {}, Light: {}, Screen: {}",
				deferredRenderer.gbufferTime, deferredRenderer.lightTime, deferredRenderer.screenTime));
	}
}
