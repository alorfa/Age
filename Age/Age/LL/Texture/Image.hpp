#pragma once

#include <filesystem>
#include "Age/Math/vec2.hpp"
#include "Age/LL/Texture/Texture2D.hpp"

namespace a_game_engine
{
	class Image
	{
		uvec2 _size;
		ubyte* _data = nullptr;
		TextureFormat _format = TextureFormat::Undefined;
	public:
		~Image();

		void loadFromMemory(const ubyte* data, int size);
		void loadFromFile(const std::filesystem::path& path);
		void clear();

		inline const ubyte* getData() const { return _data; }
		inline uvec2 getSize() const { return _size; }
		inline TextureFormat getFormat() const { return _format; }
	};
}