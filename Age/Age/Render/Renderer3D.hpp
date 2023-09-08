#pragma once

#include "Age/Math/vec2.hpp"

namespace a_game_engine
{
	class Object3D;
	class Camera3D;
	class Shader;
	class Node3D;
	class Scene3D;

	class Renderer3D
	{
	protected:
		uvec2 size;
	public:
		virtual void updateSize(const uvec2& newSize) = 0;
		virtual void drawObject(const Node3D& o, const Camera3D& c, const Shader* s) = 0;
		virtual void drawScene(const Scene3D& scene, const Camera3D& camera) = 0;
	};
}