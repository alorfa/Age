#pragma once

#include <Age/Render/ForwardRenderer.hpp>
#include <Age/Render/DeferredRenderer.hpp>
#include <Age/Scene/Scene.hpp>
#include <Age/Render/BloomEffect.hpp>

using namespace a_game_engine;

namespace a_game
{
	class WorldScene : public Scene
	{
		void setActiveRenderer(int index);

		ForwardRenderer forwardRenderer;
		DeferredRenderer deferredRenderer;
		Renderer* activeRenderer = nullptr;
	public:
		WorldScene();

		BloomEffect bloom;
		Camera* activeCamera = nullptr;
		const EnvCubeMap* env = nullptr;

		void load() override;
		void draw(const Camera* c, float delta) override;

		bool useBloom = true;
		bool useAutoExposure = true;
		bool bloomFogBlending = false;
		int bloomMipCount = 5;
		int rendererIndex = 0;

		void updateSize(uvec2 size);
		inline bool& changeDeferredRendererDebug() { return deferredRenderer.debug; }
	};
}