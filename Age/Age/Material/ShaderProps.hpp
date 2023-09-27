#pragma once

#include "ShaderProperty.hpp"
#include <vector>
#include <string>

namespace a_game_engine
{
	struct MaterialProperty
	{
		std::string name;
		ShaderProperty property;
	};
	using SceneProperty = MaterialProperty;
	using MaterialProps = std::vector<MaterialProperty>;
	using SceneProps = std::vector<SceneProperty>;
}
