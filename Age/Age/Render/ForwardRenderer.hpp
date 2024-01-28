#pragma once

#include "Renderer.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/Scene/SceneInfo.hpp"

namespace a_game_engine
{
	class ForwardRenderer : public Renderer
	{
		mutable FrameBuffer2D mainFb;
		float exposure = 1.f;

		ShaderProgram* shader;
	public:
		ForwardRenderer();

		void clear() override;
		void updateSize(const uvec2& newSize);
		void drawScene(const Scene& scene, const Camera& camera, float delta) override;
	};
}