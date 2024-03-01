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

		template <typename T>
		static T& changePixel(ImageInfo& info, uvec2 pos)
		{
			T* startData = (T*)info.data;
			if (startData == nullptr)
				return *startData;

			return startData[info.size.x * pos.y + pos.x];
		}
		template <typename T>
		static void swapRows(ImageInfo& info, uint y1, uint y2)
		{
			for (uint i = 0; i < info.size.x; i++)
			{
				std::swap(
					Image::changePixel<T>(info, { i, y1 }),
					Image::changePixel<T>(info, { i, y2 }) 
				);
			}
		}
		void flipVertically();
	};
}