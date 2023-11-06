#pragma once

#include "Age/Resource/ShaderLoader.hpp"
#include "Age/Resource/TextureLoader.hpp"
#include "Age/Resource/ModelLoader.hpp"
#include <SFML/Window/Window.hpp>
#include <Age/Transform/Camera.hpp>

namespace a_game_engine
{
	struct Egd
	{
		ShaderLoader shaders;
		TextureLoader textures;
		ModelLoader models;
		std::filesystem::path res, user;
		sf::Window* window = nullptr;
		Camera camera;
	};

	extern Egd egd;
}
