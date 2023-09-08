#include "Scene3D.hpp"

namespace a_game_engine
{
	Scene3D::Scene3D()
		: skyBox(*this)
	{
		rootNode = std::make_unique<Node3D>(*this, nullptr);
	}
	void Scene3D::handleRawEvents(const sf::Event& ev)
	{
		if (rootNode)
			rootNode->handleRawEvents(ev);
	}

	void Scene3D::handleEvents(const EventHandler& ev, float delta)
	{
		if (rootNode)
			rootNode->handleEvents(ev, delta);
	}

	void Scene3D::update(float delta)
	{
		if (rootNode)
			rootNode->update(delta);
	}
}

