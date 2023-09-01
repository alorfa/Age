#include "WorldScene.hpp"
#include <Age/Light/LightSource.hpp>
#include <SFML/Window/Event.hpp>
#include <Age/EventHandler.hpp>
#include "FollowToCamera.hpp"
#include "Age/egd.hpp"
#include "Age/Object/SkyBox.hpp"

namespace a_game
{
	WorldScene::WorldScene()
		: Node3D(nullptr)
	{
	}
	void WorldScene::load()
	{
		TextureLoader::defaultImagePath = egd.res / "img/dirt.png";
		auto nativeSize = egd.window->getSize();
		defRender.updateSize({ nativeSize.x, nativeSize.y });

		activeCamera = &egd.camera;

		//ambient = vec3{ 0.2f, 0.2f, 0.5f };
		std::unique_ptr<Object3D> objs[2] = {
			std::make_unique<Object3D>(this),
			std::make_unique<Object3D>(this)
		};
		objs[0]->model = &egd.models.load(egd.res / "model/daedric/scene.gltf",
			ModelLoader::Settings{ vec3{5.f}, false, false, true });
		objs[1]->model = &egd.models.load(egd.res / "model/kirara/scene.gltf",
			ModelLoader::Settings{ vec3{10.f}, false});
		objs[0]->shader = &egd.shaders.load(egd.res / "shader/pbrNormal");
		objs[1]->shader = &egd.shaders.load(egd.res / "shader/default");
		auto flashLight = std::make_unique<SpotLightSource>(this);
		std::unique_ptr<PointLightSource> lights[2] = { 
			std::make_unique<PointLightSource>(this), 
			std::make_unique<PointLightSource>(this) };
		lights[0]->model = lights[1]->model = 
			&egd.models.load(egd.res / "model/cube.obj");
		flashLight->shader = lights[0]->shader = lights[1]->shader = 
			&egd.shaders.load(egd.res / "shader/lightSource");
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
		auto dirLight = std::make_unique<DirLightSource>(this);

		auto skyBox = std::make_unique<SkyBox>(this);
		std::filesystem::path cubePaths[6];
		cubePaths[0] = egd.res / "img/sky/+x.jpg";
		cubePaths[1] = egd.res / "img/sky/-x.jpg";
		cubePaths[2] = egd.res / "img/sky/+y.jpg";
		cubePaths[3] = egd.res / "img/sky/-y.jpg";
		cubePaths[4] = egd.res / "img/sky/+z.jpg";
		cubePaths[5] = egd.res / "img/sky/-z.jpg";
		skyBox->cubemap = &egd.textures.loadCubeMap(cubePaths);
		skyBox->cube = egd.models.load(egd.res / "model/skybox.obj").meshes[0].get();
		skyBox->shader = &egd.shaders.load(egd.res / "shader/skybox");

		addChild(std::move(skyBox));
		addChild(std::move(objs[0]));
		addChild(std::move(objs[1]));
		addChild(std::move(lights[0]));
		addChild(std::move(lights[1]));
		addChild(std::move(flashLight));
		addChild(std::move(dirLight));

		for (auto& light : infChildren)
		{
			if (light->is<PointLightSource>())
				light->transform.changeScale() *= 0.15f;
		}
	}
	void WorldScene::handleRawEvents(const sf::Event& ev)
	{
		if (ev.type == ev.Resized)
		{
			defRender.updateSize({ ev.size.width ,ev.size.height });
		}
		Node3D::handleRawEvents(ev);
	}
	void WorldScene::handleEvents(const EventHandler& ev, float delta)
	{
		if (ev.getEvent("escape"))
			egd.window->close();
		Node3D::handleEvents(ev, delta);
	}
	void WorldScene::update(float delta)
	{
		time += delta;

		Node3D::update(delta);
	}
	void WorldScene::draw(const mat4& transform, const Node3D&, const Camera3D&, const Shader* s) const
	{
		defRender.drawScene(*this, *activeCamera);
	}
}
