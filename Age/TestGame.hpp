#pragma once

#include "Age/Application.hpp"
#include "Age/Game/WorldScene.hpp"

using namespace a_game_engine;

namespace a_game
{
	class TestGame : public Application
	{
	public:
		std::unique_ptr<WorldScene> world;

		void init() override;
		void loadResources() override;
		void handleRawEvents(const sf::Event& ev) override;
		void handleEvents(float delta) override;
		void update(float delta) override;
		void draw(float delta) const override;
		void onExit() override;
	};
}