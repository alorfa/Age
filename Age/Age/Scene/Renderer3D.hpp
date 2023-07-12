#pragma once

#include "Age/Math/vec2.hpp"

namespace a_game_engine
{
	class Scene3D;
	class Object3D;
	class Camera3D;
	class Shader;

	class Renderer3D
	{
	protected:
		uvec2 size;
	public:
		virtual void updateSize(const uvec2& newSize) = 0;
		virtual void drawObject(const Object3D& o, const Scene3D& sc,
			const Camera3D& c, const Shader* s) const = 0;
		virtual void drawScene(const Scene3D& sc) const = 0;

		static void set3DContext();
		static void set2DContext();
	};
}