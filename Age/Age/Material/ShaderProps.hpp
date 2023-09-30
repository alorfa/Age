#pragma once

#include "ShaderProperty.hpp"
#include <vector>
#include <string>

namespace a_game_engine
{
	struct TransformProps
	{
		const mat4* model, * view, * proj;
		vec3 cameraPos;
		float near, far;
	};

	class MaterialProperty
	{
	public:
		std::string name;
		ShaderProperty property;
	};
	using SceneProperty = MaterialProperty;
	using MaterialProps = std::vector<MaterialProperty>;
	using SceneProps = std::vector<SceneProperty>;
}
