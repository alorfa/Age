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
	void SceneController::update(float delta)
	{
		_time += delta;
		if (_time >= 5.f)
		{
			sf::Clock clock;
			_time = 0.f;
			_scene->rootNode->sortChildren(egd.camera.transform.getPosition(), Node::Opaque);
			Logger::logDebug(std::format("Opaque nodes has sorted in {} ms", clock.restart().asMilliseconds()));
		}
	}
}
