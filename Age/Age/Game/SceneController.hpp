#pragma once

#include <Age/Object/Component.hpp>

using namespace a_game_engine;

namespace a_game
{
	class WorldScene;

	class SceneController : public Component
	{
		WorldScene* _scene = nullptr;
	public:
		SceneController(const Node& n);

		void handleRawEvents(const sf::Event& ev) override;
		void handleEvents(const EventHandler& ev, float delta) override;
	};
}