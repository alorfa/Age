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
		virtual void clear() = 0;
		virtual void updateSize(const uvec2& newSize) = 0;
		virtual void drawScene(const Scene& scene, const Camera& camera, float delta) = 0;

		virtual ~Renderer() = default;
	};
}