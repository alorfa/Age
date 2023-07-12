#pragma once

#include "Age/Math/vec3.hpp"
#include <vector>

namespace a_game_engine
{
	class Texture2D;

	struct ColorMaterial
	{
		vec3 ambient, diffuse, specular;
		float shininess;
	};

	struct TextureMaterial
	{
		std::vector<Texture2D*> textures;
	};
}