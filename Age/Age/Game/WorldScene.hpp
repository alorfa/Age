#pragma once

#include <Age/Render/ForwardRenderer.hpp>
#include <Age/Render/DeferredRenderer.hpp>
#include <Age/Scene/Scene.hpp>

using namespace a_game_engine;

namespace a_game
{
	class WorldScene : public Scene
	{
	public:
		WorldScene();

		ForwardRenderer forwardRenderer;
		DeferredRenderer deferredRenderer;
		Renderer* activeRenderer = nullptr;
		Camera* activeCamera = nullptr;

		void load() override;
		void draw(const Camera* c) const override;
	};
}