#pragma once

#include "Age/Application.hpp"
#include "Age/Game/WorldScene.hpp"

using namespace a_game_engine;

namespace a_game
{
	class TestGame : public Application
	{
	public:
		ivec2 stopMouse;
		float sensitivity = 3.f;
		float time = 0.f;
		bool mouseIsCamera = false;
		std::unique_ptr<WorldScene> world;

		void init() override;
		void loadResources() override;
		void handleRawEvents(const sf::Event& ev) override;
		void handleEvents(float delta) override;
		void update(float delta) override;
		void draw() const override;
		void onExit() override;
	};
}