#pragma once

#include <filesystem>
#include "Age/LL/Texture/TexEnums.hpp"

namespace a_game_engine
{
	class Image
	{
		bool loadExr(const ubyte* data, int size);
		bool loadHdr(const ubyte* data, int size);
		bool loadTiff(const ubyte* data, int size);
		bool loadOthers(const ubyte* data, int size);
	public:
		ImageInfo info;
		~Image();

		void loadFromMemory(const ubyte* data, int size);
		void loadFromFile(const std::filesystem::path& path);
		void clear();
	};
}