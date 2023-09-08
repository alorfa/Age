#pragma once

#include <Age/Render/DefaultRenderer3D.hpp>
#include <Age/Scene/Scene3D.hpp>
#include <Age/Light/LightSource.hpp>

using namespace a_game_engine;

namespace a_game
{
	class WorldScene : public Scene3D
	{
		mutable DefaultRenderer3D defRender;
	public:
		WorldScene();

		Camera3D* activeCamera;

		float time = 0.f;

		void load() override;
		void handleRawEvents(const sf::Event& ev) override;
		void handleEvents(const EventHandler& ev, float delta) override;
		void update(float delta) override;
		void draw(const Camera3D& c, const Shader* s) const override;
	};
}