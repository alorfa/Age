#include "Scene.hpp"

namespace a_game_engine
{
	Scene::Scene()
		: skyBox(*this)
	{
		rootNode = std::make_unique<Node>();
		rootNode->scene = this;
	}
	void Scene::handleRawEvents(const sf::Event& ev)
	{
		if (rootNode)
			rootNode->handleRawEvents(ev);
	}

	void Scene::handleEvents(const EventHandler& ev, float delta)
	{
		if (rootNode)
			rootNode->handleEvents(ev, delta);
	}

	void Scene::update(float delta)
	{
		if (rootNode)
			rootNode->update(delta);
	}
}

