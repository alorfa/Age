#include "WorldScene.hpp"
#include <Age/Components/LightComponents.hpp>
#include <Age/Components/MeshComponent.hpp>
#include <SFML/Window/Event.hpp>
#include <Age/EventHandler.hpp>
#include "FollowToCamera.hpp"
#include "Rotate.hpp"
#include "Age/egd.hpp"
#include "Age/Material/Shader.hpp"
#include "Age/Resource/File.hpp"

namespace a_game
{
	WorldScene::WorldScene()
	{
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
		MeshComponent::setShader(*objs[0], egd.shaders.load(egd.res / "shader/pbrNormal.asl"));
		MeshComponent::setShader(*objs[1], egd.shaders.load(egd.res / "shader/default.asl"));
		//objs[0]->addComponent(std::make_unique<Rotate>(*objs[0]));
		auto spotLight = std::make_unique<SpotLightComponent>(*objs[2]);
		auto pointLight = std::make_unique<PointLightComponent>(*objs[3]);
		auto pointLight2 = std::make_unique<PointLightComponent>(*objs[4]);
		spotLight->setColor({ 0.6f, 0.6f, 1.f });
		spotLight->setAmbient({ 0.06f, 0.06f, 0.1f });
		pointLight->setColor({ 1.5f, 0.9f, 0.3f });
		pointLight->setAmbient({ 0.15f, 0.09f, 0.03f });
		pointLight2->setColor({ 1.0f, 0.1f, 0.1f });
		pointLight2->setAmbient({ 0.1f, 0.01f, 0.01f });
		objs[2]->addComponent(std::move(spotLight));
		objs[2]->addComponent(std::make_unique<FollowToCamera>(*objs[2], *activeCamera));
		objs[3]->addComponent(std::move(pointLight));
		objs[4]->addComponent(std::move(pointLight2));
		MeshComponent::addModel(*objs[3], egd.models.load(egd.res / "model/cube.obj"));
		MeshComponent::addModel(*objs[4], egd.models.load(egd.res / "model/cube.obj"));
		Shader& lightShader = egd.shaders.load(egd.res / "shader/lightSource.asl");
		lightShader.requiresEmission = true;
		MeshComponent::setShader(*objs[3], lightShader);
		MeshComponent::setShader(*objs[4], lightShader);

		objs[0]->changeTransform().changePosition() = {-3, 5, 0};
		objs[1]->changeTransform().changePosition() = { 0, 5, -1 };
		objs[3]->changeTransform().changePosition() = vec3(-1.f, 5, 2);
		objs[3]->changeTransform().changeScale() *= 0.15f;
		objs[4]->changeTransform().changePosition() = vec3(-3.f, 4, -2);
		objs[4]->changeTransform().changeScale() *= 0.15f;
		objs[5]->addComponent(std::make_unique<DirLightComponent>());

		std::filesystem::path cubePaths[6];
		cubePaths[0] = egd.res / "img/sky/+x.jpg";
		cubePaths[1] = egd.res / "img/sky/-x.jpg";
		cubePaths[2] = egd.res / "img/sky/+y.jpg";
		cubePaths[3] = egd.res / "img/sky/-y.jpg";
		cubePaths[4] = egd.res / "img/sky/+z.jpg";
		cubePaths[5] = egd.res / "img/sky/-z.jpg";
		//SkyBox::cube = egd.models.load(egd.res / "model/skybox.obj").meshes[0].get();
		//skyBox.cubemap = &egd.textures.loadCubeMap(cubePaths, TextureLoader::CubemapSettings{
		//	TextureFiltering::Linear, TextureWrap::ClampToEdge, TextureFormat::SRGB, -1});
		//skyBox.shader = &egd.shaders.loadRaw(egd.res / "shader/skybox.rasl");


		/*for (uint i = 0; i < 11; i++)
			for (uint j = 0; j < 11; j++)
			{
				auto obj = std::make_unique<Object3D>(*this, &*rootNode);
				obj->setModel(egd.models.load(egd.res / "model/smoothsus.obj",
					ModelLoader::Settings{vec3{1.f}, true}));
				obj->setShader()
			}*/

		for (uint i = 0; i < 6; i++)
			rootNode->addChild(std::move(objs[i]));
	}
	void WorldScene::handleRawEvents(const sf::Event& ev)
	{
		if (ev.type == ev.Resized)
		{
			forwardRenderer.updateSize({ ev.size.width ,ev.size.height });
			deferredRenderer.updateSize({ ev.size.width ,ev.size.height });
		}
		if (ev.type == ev.KeyPressed)
		{
			if (ev.key.code == sf::Keyboard::Left)
				activeRenderer = &forwardRenderer;
			if (ev.key.code == sf::Keyboard::Right)
				activeRenderer = &deferredRenderer;
		}
		rootNode->handleRawEvents(ev);
	}
	void WorldScene::handleEvents(const EventHandler& ev, float delta)
	{
		if (ev.getEvent("escape"))
			egd.window->close();
		rootNode->handleEvents(ev, delta);
	}
	void WorldScene::update(float delta)
	{
		rootNode->update(delta);
	}
	void WorldScene::draw(const Camera&, const ShaderProgram* s) const
	{
		activeRenderer->drawScene(*this, *activeCamera);
		if (activeRenderer == &deferredRenderer)
			egd.window->setTitle(std::format("Gbuffer: {}, Light: {}, Screen: {}",
				deferredRenderer.gbufferTime, deferredRenderer.lightTime, deferredRenderer.screenTime));
	}
}
