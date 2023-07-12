#include "Scene3D.hpp"

namespace a_game_engine
{
	void Scene3D::handleRawEvents(const sf::Event& ev)
	{
	}
	void Scene3D::handleEvents(const EventHandler& ev, float delta)
	{
	}
	void Scene3D::update(float delta)
	{
		for (const auto& obj : objects)
			obj->update(delta);
		for (const auto& obj : pointLights)
			obj->update(delta);
		for (const auto& obj : spotLights)
			obj->update(delta);
	}
	void Scene3D::draw() const
	{
		for (const auto& obj : objects)
			obj->draw(*this, *activeCamera);
		for (const auto& light : pointLights)
			light->draw(*this, *activeCamera);
	}
}
