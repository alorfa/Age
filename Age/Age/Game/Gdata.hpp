#pragma once

#include <Age/Resource/ModelLoader.hpp>
#include <Age/Resource/ShaderLoader.hpp>
#include <Age/Resource/TextureLoader.hpp>
#include <SFML/Window/Window.hpp>
#include <Age/Transform/Camera3D.hpp>

using namespace a_game_engine;

namespace a_game
{
	struct Gdata
	{
		std::filesystem::path res, user;
		ModelLoader modelLoader;
		ShaderLoader shaderLoader;
		TextureLoader textureLoader;
		sf::Window* window = nullptr;
		Camera3D camera;

		Gdata();
	};

	extern Gdata* gdata;
}