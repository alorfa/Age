#pragma once

#include "Age/types.hpp"
#include <filesystem>

namespace a_game_engine
{
	class ShaderCompiler
	{
	public:
		static const std::string glslVersionString;

		enum Type { Vertex, Fragment };

		static uint loadFromMemory(const std::string& data, int type);
		static uint loadFromFile(const std::filesystem::path& path, int type);
	};
}