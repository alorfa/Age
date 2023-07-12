#pragma once

#include "vec3.hpp"

namespace a_game_engine
{
	struct mat4
	{
		float data[16];

		mat4();

		mat4& operator*=(const mat4& m2);
		mat4 operator*(const mat4& m2) const;

		void reflect();
	};
}