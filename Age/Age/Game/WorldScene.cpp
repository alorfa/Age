#include "WorldScene.hpp"
#include "Gdata.hpp"
#include "Cube.hpp"
#include "Monkie.hpp"
#include <Age/Light/LightSource.hpp>
#include <SFML/Window/Event.hpp>
#include <Age/EventHandler.hpp>

namespace a_game
{
	WorldScene::WorldScene()
		: Node3D(nullptr)
	{
	}
	void WorldScene::load()
	{
		auto nativeSize = gdata->window->getSize();
		defRender.updateSize({ nativeSize.x, nativeSize.y });

		activeCamera = &gdata->camera;

		//ambient = vec3{ 0.2f, 0.2f, 0.5f };
		auto cube = std::make_unique<Cube>(this);
		auto monkie = std::make_unique<Monkie>(this);
		auto flashLight = std::make_unique<SpotLightSource>(this);
		spotLight = flashLight.get();
		std::unique_ptr<PointLightSource> lights[2] = { 
			std::make_unique<PointLightSource>(this), 
			std::make_unique<PointLightSource>(this) };
		lights[0]->model = lights[1]->model = 
			&gdata->modelLoader.load(gdata->res / "model/cube.obj");
		flashLight->shader = lights[0]->shader = lights[1]->shader = 
			&gdata->shaderLoader.load(gdata->res / "shader/lightSource");

		lights[0]->transform.changePosition() = vec3(-1.f, 5, 2);
		lights[1]->transform.changePosition() = vec3(-3.f, 4, -2);
		lights[0]->light.color = vec3(1.5f, 0.9f, 0.3f);
		//lights[0]->light.color = vec3(0.0f, 0.0f, 0.0f);
		lights[0]->light.ambient = lights[0]->light.color * 0.1f;
		lights[1]->light.color = vec3(1.0f, 0.1f, 0.1f);
		//lights[1]->light.color = vec3(0.0f, 0.f, 0.f);
		lights[1]->light.ambient = lights[1]->light.color * 0.1f;
		cube->transform.changePosition() = { -3, 5, 0 };
		monkie->transform.changePosition() = { 0, 5, -1 };
		flashLight->light.color = { 0.6f, 0.6f, 1.f };
		flashLight->light.ambient = flashLight->light.color * 0.1f;
		auto dirLight = std::make_unique<DirLightSource>(this);

		addChild(std::move(cube));
		addChild(std::move(monkie));
		addChild(std::move(lights[0]));
		addChild(std::move(lights[1]));
		addChild(std::move(flashLight));
		addChild(std::move(dirLight));

		//ambient = { 0.8f };

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
	}
	void WorldScene::handleEvents(const EventHandler& ev, float delta)
	{
		if (ev.getEvent("escape"))
			gdata->window->close();
	}
	void WorldScene::update(float delta)
	{
		time += delta;

		/*auto& dirl = *dirLights.begin();
		dirl.dir = { 0.f, sinf(time), cosf(time) };

		dirl.color = { 0.4f, 0.4f, 0.6f };
		dirl.ambient = dirl.color * 0.1f;*/
		/*dirl.color.z = -sinf(time);
		dirl.color.x = std::max(-cosf(time), 0.f);*/

		spotLight->transform.changePosition() = activeCamera->transform.getPosition();
		spotLight->transform.changeRotation() = activeCamera->transform.getRotation();
	}
	void WorldScene::draw(const mat4& transform, const Node3D&, const Camera3D&, const Shader* s) const
	{
		defRender.drawScene(*this, *activeCamera);
	}
}
