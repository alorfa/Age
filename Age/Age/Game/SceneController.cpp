#include "SceneController.hpp"
#include <Age/Object/Node.hpp>
#include <Age/Game/WorldScene.hpp>
#include <Age/EventHandler.hpp>
#include <Age/egd.hpp>

namespace a_game
{
	SceneController::SceneController(const Node& n)
	{
		_scene = dynamic_cast<WorldScene*>(n.scene);
	}

	void SceneController::handleRawEvents(const sf::Event& ev)
	{
		if (ev.type == ev.Resized)
		{
			_scene->forwardRenderer.updateSize({ ev.size.width ,ev.size.height });
			_scene->deferredRenderer.updateSize({ ev.size.width ,ev.size.height });
		}
		if (ev.type == ev.KeyPressed)
		{
			if (ev.key.code == sf::Keyboard::Left)
				_scene->activeRenderer = &_scene->forwardRenderer;
			if (ev.key.code == sf::Keyboard::Right)
				_scene->activeRenderer = &_scene->deferredRenderer;
		}
	}
	void SceneController::handleEvents(const EventHandler& ev, float delta)
	{
		if (ev.getEvent("escape"))
			egd.window->close();
	}
}
