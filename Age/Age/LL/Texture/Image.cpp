#include "Image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Age/Resource/File.hpp"

namespace a_game_engine
{
	Image::~Image()
	{
		clear();
	}
	void Image::loadFromMemory(const ubyte* data, int size)
	{
		stbi_set_flip_vertically_on_load(true);
		clear();
		int components, x, y;
		_data = stbi_load_from_memory(data, size, &x, &y, &components, 0); //TODO: log if null
		_size.x = (uint)x;
		_size.y = (uint)y;
		_format = TextureFormat(components);
	}
	void Image::loadFromFile(const std::filesystem::path& path)
	{
		auto bytes = File::readAllBytes(path);
		if (bytes.size())
			loadFromMemory(bytes.data(), (int)bytes.size());
	}
	void Image::clear()
	{
		if (_data)
		{
			stbi_image_free(_data);
			_data = nullptr;
			_size.x = _size.y = 0;
			_format = TextureFormat::Undefined;
		}
	}
}