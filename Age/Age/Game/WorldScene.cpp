#include "WorldScene.hpp"
#include <Age/Light/LightSource.hpp>
#include <SFML/Window/Event.hpp>
#include <Age/EventHandler.hpp>
#include "FollowToCamera.hpp"
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

		activeCamera = &egd.camera;

		ShaderSettings::Forward settings;
		settings.dirLights = settings.spotLights = 1;
		settings.pointLights = 2;

		//ambient = vec3{ 0.2f, 0.2f, 0.5f };
		std::unique_ptr<Object3D> objs[2] = {
			std::make_unique<Object3D>(*this, &*rootNode),
			std::make_unique<Object3D>(*this, &*rootNode)
		};
		objs[0]->model = &egd.models.load(egd.res / "model/daedric/scene.gltf",
			ModelLoader::Settings{ vec3{5.f}, false, false, true });
		objs[1]->model = &egd.models.load(egd.res / "model/kirara/scene.gltf",
			ModelLoader::Settings{ vec3{10.f}, false});
		//objs[0]->shader = &egd.shaders.load(egd.res / "shader/pbrNormal");
		//objs[1]->shader = &egd.shaders.load(egd.res / "shader/default");
		objs[0]->setShader(egd.shaders.load(egd.res / "shader/pbrNormal.asl"));
		objs[1]->setShader(egd.shaders.load(egd.res / "shader/default.asl"));
		auto flashLight = std::make_unique<SpotLightSource>(*this, &*rootNode);
		std::unique_ptr<PointLightSource> lights[2] = { 
			std::make_unique<PointLightSource>(*this, &*rootNode),
			std::make_unique<PointLightSource>(*this, &*rootNode) };
		lights[0]->model = lights[1]->model = 
			&egd.models.load(egd.res / "model/cube.obj");
		const Shader& lightShader = egd.shaders.load(egd.res / "shader/lightSource.asl");
		flashLight->setShader(lightShader);
		lights[0]->setShader(lightShader);
		lights[1]->setShader(lightShader);
		flashLight->addComponent(std::make_unique<FollowToCamera>(*flashLight, *activeCamera));

		lights[0]->transform.changePosition() = vec3(-1.f, 5, 2);
		lights[1]->transform.changePosition() = vec3(-3.f, 4, -2);
		lights[0]->light.color = vec3(1.5f, 0.9f, 0.3f);
		//lights[0]->light.color = vec3(0.0f, 0.0f, 0.0f);
		lights[0]->light.ambient = lights[0]->light.color * 0.1f;
		lights[1]->light.color = vec3(1.0f, 0.1f, 0.1f);
		//lights[1]->light.color = vec3(0.0f, 0.f, 0.f);
		lights[1]->light.ambient = lights[1]->light.color * 0.1f;
		objs[0]->transform.changePosition() = {-3, 5, 0};
		objs[1]->transform.changePosition() = { 0, 5, -1 };
		flashLight->light.color = { 0.6f, 0.6f, 1.f };
		flashLight->light.ambient = flashLight->light.color * 0.1f;
		auto dirLight = std::make_unique<DirLightSource>(*this, &*rootNode);

		std::filesystem::path cubePaths[6];
		cubePaths[0] = egd.res / "img/sky/+x.jpg";
		cubePaths[1] = egd.res / "img/sky/-x.jpg";
		cubePaths[2] = egd.res / "img/sky/+y.jpg";
		cubePaths[3] = egd.res / "img/sky/-y.jpg";
		cubePaths[4] = egd.res / "img/sky/+z.jpg";
		cubePaths[5] = egd.res / "img/sky/-z.jpg";
		SkyBox::cube = egd.models.load(egd.res / "model/skybox.obj").meshes[0].get();
		skyBox.cubemap = &egd.textures.loadCubeMap(cubePaths, TextureLoader::CubemapSettings{
			TextureFiltering::Linear, TextureWrap::ClampToEdge, TextureFormat::SRGB, -1});
		skyBox.shader = &egd.shaders.loadRaw(egd.res / "shader/skybox.rasl");

		rootNode->addChild(std::move(objs[0]));
		rootNode->addChild(std::move(objs[1]));
		rootNode->addChild(std::move(lights[0]));
		rootNode->addChild(std::move(lights[1]));
		rootNode->addChild(std::move(flashLight));
		rootNode->addChild(std::move(dirLight));

		for (auto& light : rootNode->infChildren)
		{
			if (light->is<PointLightSource>())
				light->transform.changeScale() *= 0.15f;
		}
	}
	void WorldScene::handleRawEvents(const sf::Event& ev)
	{
		if (ev.type == ev.Resized)
		{
			forwardRenderer.updateSize({ ev.size.width ,ev.size.height });
			deferredRenderer.updateSize({ ev.size.width ,ev.size.height });
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
	void WorldScene::draw(const Camera3D&, const ShaderProgram* s) const
	{
		forwardRenderer.drawScene(*this, *activeCamera);
		//deferredRenderer.drawScene(*this, *activeCamera);
	}
}
