#include "SceneController.hpp"
#include <Age/Object/Node.hpp>
#include <Age/Game/WorldScene.hpp>
#include <Age/EventHandler.hpp>
#include <Age/egd.hpp>
#include <Age/Other/Logger.hpp>

namespace a_game
{
	SceneController::SceneController(const Node& n)
	{
		_scene = dynamic_cast<WorldScene*>(n.scene);
		_windowSize = uvec2(egd.window->getSize().x, egd.window->getSize().y);
	}

	void SceneController::handleRawEvents(const sf::Event& ev)
	{
		if (ev.type == ev.Resized)
		{
			_windowSize = { ev.size.width ,ev.size.height };
			_scene->activeRenderer->updateSize(_windowSize);
		}
	}
	void SceneController::handleEvents(const EventHandler& ev, float delta)
	{
		if (ev.getEvent("escape"))
			egd.window->close();
	}
	void SceneController::update(float delta)
	{
		_time += delta;
		if (_time >= 5.f)
		{
			_time = 0.f;
			_scene->rootNode->sortChildren(egd.camera.transform.getPosition(), Node::Opaque);
		}
	}
}
