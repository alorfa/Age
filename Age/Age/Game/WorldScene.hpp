#pragma once

#include <Age/Render/ForwardRenderer.hpp>
#include <Age/Render/DeferredRenderer.hpp>
#include <Age/Scene/Scene.hpp>
#include <Age/Light/LightSource.hpp>

using namespace a_game_engine;

namespace a_game
{
	class WorldScene : public Scene
	{
		mutable ForwardRenderer forwardRenderer;
		mutable DeferredRenderer deferredRenderer;
		Renderer* activeRenderer;
	public:
		WorldScene();

		Camera* activeCamera;

		float time = 0.f;

		void load() override;
		void handleRawEvents(const sf::Event& ev) override;
		void handleEvents(const EventHandler& ev, float delta) override;
		void update(float delta) override;
		void draw(const Camera& c, const ShaderProgram* s) const override;
	};
}