#pragma once

#include <Age/Object/Component.hpp>

using namespace a_game_engine;

namespace a_game
{
	class WorldScene;

	class PlayerController : public Component
	{
		WorldScene* _scene = nullptr;
		ivec2 stopMouse;
		float sensitivity = 3.f;
		bool enableControl = false;
	public:
		PlayerController(const Node& n);

		void handleRawEvents(const sf::Event& ev) override;
		void handleEvents(const EventHandler& ev, float delta) override;

		void setCameraActive(bool value);
	};
}