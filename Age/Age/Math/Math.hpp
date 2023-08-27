#pragma once

#include "vec3.hpp"

namespace a_game_engine
{
	class Math
	{
	public:
		static const float PI;

		static float sin(float value);
		static float cos(float value);

		static float rad(float deg);
		static float deg(float rad);

		static vec3 getForwardDir(const vec3& euler);
		static vec3 getRightDir(const vec3& euler);
		static vec3 getUpDir(const vec3& euler);
	};
}
