#include "WorldScene.hpp"
#include "Gdata.hpp"
#include "Cube.hpp"
#include "Monkie.hpp"
#include <Age/Light/LightSource.hpp>

namespace a_game
{
	void WorldScene::load()
	{
		auto nativeSize = gdata->window->getSize();
		defRender.updateSize({ nativeSize.x, nativeSize.y });

		activeCamera = &gdata->camera;

		//ambient = vec3{ 0.2f, 0.2f, 0.5f };
		auto cube = std::make_unique<Cube>();
		auto monkie = std::make_unique<Monkie>();
		std::unique_ptr<PointLightSource> lights[2] = {
			std::make_unique<PointLightSource>( gdata->modelLoader.load(gdata->res / "model/cube.obj"), 
			gdata->shaderLoader.load(gdata->res / "shader/lightSource") ),
			std::make_unique<PointLightSource>(gdata->modelLoader.load(gdata->res / "model/cube.obj"),
			gdata->shaderLoader.load(gdata->res / "shader/lightSource") )
		}; 
		DirLight dirLight;
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
		auto flashLight = std::make_unique<SpotLightSource>(
			gdata->modelLoader.load(gdata->res / "model/cube.obj"), 
			gdata->shaderLoader.load(gdata->res / "shader/lightSource"));
		flashLight->light.color = { 0.6f, 0.6f, 1.f };
		flashLight->light.ambient = flashLight->light.color * 0.1f;

		objects.push_front(std::move(cube));
		objects.push_front(std::move(monkie));
		pointLights.push_front(std::move(lights[0]));
		pointLights.push_front(std::move(lights[1]));
		spotLights.push_front(std::move(flashLight));
		dirLights.push_front(dirLight);

		//ambient = { 0.8f };

		for (auto& light : pointLights)
			light->transform.changeScale() *= 0.15f;
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

		auto& dirl = *dirLights.begin();
		dirl.dir = { 0.f, sinf(time), cosf(time) };

		dirl.color = { 0.4f, 0.4f, 0.6f };
		dirl.ambient = dirl.color * 0.1f;
		/*dirl.color.z = -sinf(time);
		dirl.color.x = std::max(-cosf(time), 0.f);*/

		auto& flashl = *spotLights.begin();
		flashl->transform.changePosition() = activeCamera->transform.getPosition();
		flashl->transform.changeRotation() = activeCamera->transform.getRotation();

		Scene3D::update(delta);
	}
	void WorldScene::draw() const
	{
		defRender.drawScene(*this);
	}
}
