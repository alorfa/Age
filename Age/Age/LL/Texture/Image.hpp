#pragma once

#include <filesystem>
#include "Age/LL/Texture/TexEnums.hpp"

namespace a_game_engine
{
	class Image
	{
	public:
		ImageInfo info;
		~Image();

		void loadFromMemory(const ubyte* data, int size);
		void loadFromFile(const std::filesystem::path& path);
		void clear();
	};
}