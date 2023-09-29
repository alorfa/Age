#pragma once

#include "Age/Material/ShaderProps.hpp"
#include "Age/types.hpp"
#include "Age/Material/ShaderSettings.hpp"

namespace a_game_engine
{
	class Camera3D;
	class Shader;
	class Node3D;

	struct Scene3DInfo
	{
		struct LightsInfo
		{
			uint dir = 0, point = 0, spot = 0;
		};

		const Camera3D* camera = nullptr;
		const Shader* shader = nullptr;
		SceneProps props;
		LightsInfo lights;
		ShaderSettings::Common shaderSettings;

		void addLights(const Node3D& node);
	};
}
