#pragma once

#include "Renderer3D.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"

namespace a_game_engine
{
	class DefaultRenderer3D : public Renderer3D
	{
		mutable FrameBuffer2D mainFb;

		ShaderProgram* shader;
	public:
		DefaultRenderer3D();

		void updateSize(const uvec2& newSize);
		void drawObject(const Node3D& o, const Camera3D& c, const ShaderProgram* s) override;
		void drawScene(const Scene3D& scene, const Camera3D& camera) override;
	};
}