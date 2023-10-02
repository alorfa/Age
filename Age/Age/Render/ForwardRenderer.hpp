#pragma once

#include "Renderer3D.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/Scene/Scene3DInfo.hpp"

namespace a_game_engine
{
	class ForwardRenderer : public Renderer3D
	{
		mutable FrameBuffer2D mainFb;

		ShaderProgram* shader;
	public:
		ForwardRenderer();

		void updateSize(const uvec2& newSize);
		void drawObject(const Node3D& o, const Scene3DInfo& info) override;
		void drawScene(const Scene3D& scene, const Camera3D& camera) override;
	};
}