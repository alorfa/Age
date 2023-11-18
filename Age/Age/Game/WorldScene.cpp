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

namespace a_game
{
	WorldScene::WorldScene()
	{
		rootNode->addComponent<SceneController>();
	}
	void WorldScene::load()
	{
		auto nativeSize = egd.window->getSize();
		forwardRenderer.updateSize({ nativeSize.x, nativeSize.y });
		deferredRenderer.updateSize({ nativeSize.x, nativeSize.y });
		activeRenderer = &forwardRenderer;

		activeCamera = &egd.camera;

		std::unique_ptr<Node> objs[6];
		for (uint i = 0; i < 6; i++)
			objs[i] = std::make_unique<Node>();
		MeshComponent::addModel(*objs[0], egd.models.load(egd.res / "model/daedric/scene.gltf",
			ModelLoader::Settings{ vec3{5.f}, false, false, true }));
		MeshComponent::addModel(*objs[1], egd.models.load(egd.res / "model/kirara/scene.gltf",
			ModelLoader::Settings{ vec3{10.f}, false}));
		MeshComponent::addModel(*objs[3], egd.models.load(egd.res / "model/cube.obj"));
		MeshComponent::addModel(*objs[4], egd.models.load(egd.res / "model/cube.obj"));
		MeshComponent::setShader(*objs[0], egd.shaders.load(egd.res / "shader/pbrNormal.asl"));
		MeshComponent::setShader(*objs[1], egd.shaders.load(egd.res / "shader/default.asl"));
		Shader& lightShader = egd.shaders.load(egd.res / "shader/lightSource.asl");
		lightShader.requiresEmission = true;
		MeshComponent::setShader(*objs[3], lightShader);
		MeshComponent::setShader(*objs[4], lightShader);
		//objs[0]->addComponent(std::make_unique<Rotate>(*objs[0]));

		objs[2]->addComponent<SpotLightComponent>()
			.setColor({ 1.f, 1.f, 2.4f }, 0.05f);
		objs[2]->addComponent<FollowToCamera>()
			.setCamera(*activeCamera);

		objs[3]->addComponent<PointLightComponent>()
			.setColor({ 2.f, 1.2f, 0.5f }, 0.0f)
			.addModel(*objs[3]);
		objs[3]->forEach([](Node& n) {
			auto meshes = n.findAllComponents<MeshComponent>();
			for (auto& m : meshes)
				m->mesh.material.setValue("emission", ShaderProperty(vec3{0.f}));
		});
		objs[4]->addComponent<PointLightComponent>()
			.setColor({ 1.0f, 0.1f, 0.1f }, 0.f)
			.addModel(*objs[4]);
		objs[5]->addComponent<DirLightComponent>();

		objs[0]->changeTransform().changePosition() = { -3, 5, 0 };
		objs[1]->changeTransform().changePosition() = { 0, 5, -1 };
		objs[3]->changeTransform().changePosition() = vec3(-1.f, 5, 2);
		objs[3]->changeTransform().changeScale() *= 0.15f;
		objs[4]->changeTransform().changePosition() = vec3(-3.f, 4, -2);
		objs[4]->changeTransform().changeScale() *= 0.15f;

		std::filesystem::path cubePaths[6];
		cubePaths[0] = egd.res / "img/sky/+x.jpg";
		cubePaths[1] = egd.res / "img/sky/-x.jpg";
		cubePaths[2] = egd.res / "img/sky/+y.jpg";
		cubePaths[3] = egd.res / "img/sky/-y.jpg";
		cubePaths[4] = egd.res / "img/sky/+z.jpg";
		cubePaths[5] = egd.res / "img/sky/-z.jpg";
		SkyBox::cube = &egd.models.load(egd.res / "model/skybox.obj").meshes[0].get()->buffer;
		skyBox.cubemap = &egd.textures.loadCubeMap(cubePaths, TextureLoader::CubemapSettings{
			TextureFiltering::Linear, TextureWrap::ClampToEdge, TextureFormat::SRGB, -1});
		skyBox.shader = &egd.shaders.loadRaw(egd.res / "shader/skybox.rasl");

		for (uint i = 0; i < 11; i++)
			for (uint j = 0; j < 11; j++)
			{
				auto& sphere = rootNode->addChild();
				MeshComponent::addModel(sphere, egd.models.load(egd.res / "model/smoothsus.obj",
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
				sphere.setScale({ 0.3f });
			}
	

		for (uint i = 0; i < 6; i++)
			rootNode->addChild(std::move(objs[i]));
	}
	void WorldScene::draw(const Camera* c) const
	{
		const Camera* camera = c ? c : activeCamera;
		activeRenderer->drawScene(*this, *camera);
		if (activeRenderer == &deferredRenderer)
			egd.window->setTitle(std::format("Gbuffer: {}, Light: {}, Screen: {}",
				deferredRenderer.gbufferTime, deferredRenderer.lightTime, deferredRenderer.screenTime));
	}
}
