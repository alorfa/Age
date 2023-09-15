#pragma once

#include <string>
#include "Age/types.hpp"

namespace a_game_engine
{
	class ShaderProgram;

	class Shader
	{
		std::string _source;
	public:
		const ShaderProgram& getForward(uint dirLights, uint pointLights, uint spotLights) const;
		const ShaderProgram& getDeferred() const;
	};
}
