#pragma once

#include "Age/Math/vec2.hpp"

namespace a_game_engine
{
	class Camera;
	class ShaderProgram;
	class Node;
	class Scene;
	struct SceneInfo;

	class Renderer
	{
	protected:
		uvec2 size;
	public:
		virtual void updateSize(const uvec2& newSize) = 0;
		virtual void drawObject(const Node& o, const SceneInfo& info) = 0;
		virtual void drawScene(const Scene& scene, const Camera& camera) = 0;
	};
}