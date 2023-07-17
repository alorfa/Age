#pragma once

#include <filesystem>
#include "Age/LL/Texture/TexEnums.hpp"

namespace a_game_engine
{
	class Image
	{
		ImageInfo _img;
	public:
		~Image();

		void loadFromMemory(const ubyte* data, int size);
		void loadFromFile(const std::filesystem::path& path);
		void clear();

		inline const ImageInfo& getInfo() const { return _img; }
	};
}