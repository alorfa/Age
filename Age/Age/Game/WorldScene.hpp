#pragma once

#include <Age/Scene/Scene3D.hpp>
#include <Age/Scene/DefaultRenderer3D.hpp>

using namespace a_game_engine;

namespace a_game
{
	class WorldScene : public Scene3D
	{
		DefaultRenderer3D defRender;
	public:
		WorldScene() = default;

		ColorMaterial material;
		float time = 0.f;

		void load() override;
		void handleRawEvents(const sf::Event& ev) override;
		void handleEvents(const EventHandler& ev, float delta) override;
		void update(float delta) override;
		void draw() const override;
	};
}