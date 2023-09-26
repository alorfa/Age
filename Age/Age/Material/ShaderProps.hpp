#pragma once

#include "ShaderProperty.hpp"
#include <vector>
#include <string>

namespace a_game_engine
{
	class MaterialProps
	{
	public:
		struct Property
		{
			std::string name;
			ShaderProperty property;
		};
		std::vector<Property> props;
	};
	using SceneProps = MaterialProps;
}
