#pragma once

#include "Age/types.hpp"
#include <filesystem>

namespace a_game_engine
{
	class ShaderCompiler
	{
	public:
		enum Type { Vertex, Fragment };

		static uint loadFromMemory(const char* data, int type);
		static uint loadFromFile(const std::filesystem::path& path, int type);
	};
}