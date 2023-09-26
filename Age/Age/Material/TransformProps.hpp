#pragma once

#include "Age/Math/mat4.hpp"
#include "Age/Math/vec3.hpp"
#include "Age/Math/vec2.hpp"

namespace a_game_engine
{
	struct TransformProps
	{
		const mat4 *model, *view, *proj;
		vec3 cameraPos;
		float near, far;
	};
}