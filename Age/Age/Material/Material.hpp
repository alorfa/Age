#pragma once

#include "ShaderProps.hpp"

namespace a_game_engine
{
	class Shader;

	struct Material
	{
		MaterialProps props;
		const Shader* shader = nullptr;

		void setValue(const std::string& name, const ShaderProperty& prop);
	};
}