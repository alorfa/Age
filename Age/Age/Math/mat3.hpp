#pragma once

namespace a_game_engine
{
	struct mat3
	{
		float data[9];

		mat3();

		mat3& operator*=(const mat3& m2);
		mat3 operator*(const mat3& m2);
	};
}