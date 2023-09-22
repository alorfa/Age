#pragma once

#include <vector>
#include <filesystem>
#include "Age/Resource/Logger.hpp"
#include "Age/types.hpp"

namespace a_game_engine
{
	class File
	{
	public:
		static std::vector<ubyte> readAllBytes(const std::filesystem::path& path);
		static std::string readAllText(const std::filesystem::path& path);
		static void writeToFile(const std::filesystem::path& path, const std::string& s);
	};
}