#include "Light.hpp"
#include <cmath>

namespace a_game_engine
{
	float DirLight::computeScatter(float radius)
	{
		return 1.f - cosf(atanf(radius));
	}
}

