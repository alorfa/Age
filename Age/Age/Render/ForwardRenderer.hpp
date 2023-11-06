#pragma once

#include "Renderer.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/Scene/SceneInfo.hpp"

namespace a_game_engine
{
	class ForwardRenderer : public Renderer
	{
		mutable FrameBuffer2D mainFb;

		ShaderProgram* shader;
	public:
		ForwardRenderer();

		void updateSize(const uvec2& newSize);
		void drawObject(const Node& o, const SceneInfo& info) override;
		void drawScene(const Scene& scene, const Camera& camera) override;
	};
}