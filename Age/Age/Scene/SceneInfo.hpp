#pragma once

#include "Age/Material/ShaderProps.hpp"
#include "Age/types.hpp"
#include "Age/Material/ShaderSettings.hpp"
#include <functional>

namespace a_game_engine
{
	class Camera;
	class Shader;
	class Node;
	struct Material;

	struct SceneInfo
	{
		struct LightsInfo
		{
			uint dir = 0, point = 0, spot = 0;
		};

		const Camera* camera = nullptr;
		const Shader* shader = nullptr;
		SceneProps props;
		LightsInfo lights;
		ShaderSettings::Common shaderSettings;
		std::function<bool(const Material&)> drawingCondition;

		void addLights(const Node& node);
	};
}
