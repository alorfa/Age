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
			uint dir = 0, point = 0, spot = 0, shadowDir = 0;
		};

		const Shader* shader = nullptr;
		mat4 projView;
		vec3 cameraPos;
		vec2 nearFar;
		SceneProps props;
		LightsInfo lights;
		ShaderSettings::Common shaderSettings;
		std::function<bool(const Material&)> drawingCondition;

		void addLights(const Node& node, int useShadows = 0);
		void setCamera(const Camera& c);
	};
}
