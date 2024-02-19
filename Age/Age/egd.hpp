#pragma once

#include "Age/Resource/ShaderLoader.hpp"
#include "Age/Resource/CubeMapLoader.hpp"
#include "Age/Resource/ModelLoader.hpp"
#include <SFML/Window/Window.hpp>
#include <Age/Transform/Camera.hpp>

namespace a_game_engine
{
	struct Egd
	{
		ShaderLoader shaders;
		TextureLoader textures;
		CubeMapLoader cubemaps;
		ModelLoader models;
		std::filesystem::path res, user;
		sf::Window* window = nullptr;
		Camera camera;
	};

	extern Egd egd;
}
