#pragma once

#include "Age/Material/ShaderProps.hpp"
#include "Age/types.hpp"

namespace a_game_engine
{
	class Camera3D;
	class ShaderProgram;
	class Node3D;

	struct Scene3DInfo
	{
		struct LightsInfo
		{
			uint dir = 0, point = 0, spot = 0;
		};

		const Camera3D* camera = nullptr;
		const ShaderProgram* shader = nullptr;
		SceneProps props;
		LightsInfo lights;

		void addLights(const Node3D& node);
	};
}
