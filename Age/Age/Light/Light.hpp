#pragma once

#include "Age/Math/vec3.hpp"

namespace a_game_engine
{
	struct DirLight
	{
		vec3 dir = { 0.f, 0.f, 1.f }, ambient, color = { 1.f, 1.f, 1.f };
		float radius = 0.1f;
	};

	struct PointLight
	{
		vec3 pos, ambient, color = { 1.f, 1.f, 1.f };
		float constant = 1.f, linear = 0.09f, quadratic = 0.032f;
		float radius = 0.1f;
	};

	struct SpotLight
	{
		vec3 pos, dir = { 0.f, 0.f, -1.f }; float cutOff = 0.1f, outerCutOff = 0.26f;
		vec3 ambient, color = { 1.f, 1.f, 1.f };
		float constant = 1.f, linear = 0.09f, quadratic = 0.032f;
		float radius = 0.1f;
	};
}