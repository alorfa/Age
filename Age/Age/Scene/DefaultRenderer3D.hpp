#pragma once

#include "Renderer3D.hpp"
#include "Age/LL/Buffers/FrameBuffer.hpp"

namespace a_game_engine
{
	class DefaultRenderer3D : public Renderer3D
	{
		mutable FrameBuffer2D mainFb;

		Shader* shader;
	public:
		DefaultRenderer3D();

		void updateSize(const uvec2& newSize);
		void drawObject(const Object3D& o, const Scene3D& sc,
			const Camera3D& c, const Shader* s) const override;
		void drawScene(const Scene3D& sc) const override;
	};
}