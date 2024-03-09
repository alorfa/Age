#pragma once

#include <Age/Render/ForwardRenderer.hpp>
#include <Age/Render/DeferredRenderer.hpp>
#include <Age/Scene/Scene.hpp>

using namespace a_game_engine;

namespace a_game
{
	class WorldScene : public Scene
	{
		void setActiveRenderer(int index);
	public:
		WorldScene();

		ForwardRenderer forwardRenderer;
		DeferredRenderer deferredRenderer;
		Renderer* activeRenderer = nullptr;
		Camera* activeCamera = nullptr;
		const EnvCubeMap* env = nullptr;

		void load() override;
		void draw(const Camera* c, float delta) override;

		bool useBloom = true;
		bool useAutoExposure = true;
		int bloomMipCount = 5;
		float bloomRadius = 1.5f;
		int rendererIndex = 0;
	};
}