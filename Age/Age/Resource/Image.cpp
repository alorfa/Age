#include "Image.hpp"
#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Age/Resource/File.hpp"
#include "Age/Math/vec3.hpp"
#include "Age/Math/vec4.hpp"

namespace a_game_engine
{
	namespace
	{
		template <typename T>
		void tFlipVertically(ImageInfo& info)
		{
			for (uint y = 0; y < info.size.y / 2; y++)
			{
				Image::swapRows<T>(info, y, info.size.y - 1 - y);
			}
		}
	}

	bool Image::loadExr(const ubyte* data, int size)
	{
		const char* err = nullptr;
		EXRHeader header;
		InitEXRHeader(&header); 
		EXRVersion version;
		int versionresult = ParseEXRVersionFromMemory(&version, data, size);
		if (versionresult)
			return false;

		EXRImage img;
		InitEXRImage(&img);
		int headerResult = ParseEXRHeaderFromMemory(&header, &version, data, size, &err);
		const int channels = header.num_channels;
		FreeEXRHeader(&header);
		if (headerResult)
		{
			if (err)
				FreeEXRErrorMessage(err);
			return false;
		}
		if (channels == 0 or channels > 4)
			return false;

		float* imageData = nullptr;
		uint* x = &info.size.x;
		uint* y = &info.size.y;
		int loadResult = LoadEXRFromMemory(&imageData, (int*)x, (int*)y, data, size, &err);
		info.data = (ubyte*)imageData;
		if (loadResult)
		{
			if (err)
				FreeEXRErrorMessage(err);
			return false;
		}
		switch (channels)
		{
		case 1:
			info.format = TextureFormat::R_F32; break;
		case 2:
			info.format = TextureFormat::RG_F32; break;
		case 3:
			info.format = TextureFormat::RGB_F32; break;
		case 4:
			info.format = TextureFormat::RGBA_F32; break;
		}
		flipVertically();
		return true;
	}
	bool Image::loadHdr(const ubyte* data, int size)
	{
		return false;
	}
	bool Image::loadTiff(const ubyte* data, int size)
	{
		return false;
	}
	bool Image::loadOthers(const ubyte* data, int size)
	{
		stbi_set_flip_vertically_on_load(true);
		clear();
		int components, x, y;
		info.data = stbi_load_from_memory(data, size, &x, &y, &components, 0); //TODO: log if null
		if (info.data == nullptr)
			return false;

		info.size.x = (uint)x;
		info.size.y = (uint)y;
		info.format = TextureFormat(components);
		return true;
	}

	Image::~Image()
	{
		clear();
	}
	void Image::loadFromMemory(const ubyte* data, int size)
	{
		if (loadExr(data, size))
			return;
		loadOthers(data, size);
	}
	void Image::loadFromFile(const std::filesystem::path& path)
	{
		auto bytes = File::readAllBytes(path);
		if (bytes.size())
			loadFromMemory(bytes.data(), (int)bytes.size());
	}
	void Image::clear()
	{
		if (info.data)
		{
			stbi_image_free(info.data);
			info.data = nullptr;
		}
		info.size.x = info.size.y = 0;
		info.format = TextureFormat::Undefined;
	}

	void Image::flipVertically()
	{
		if (not info.isValid())
			return;

		switch (info.format)
		{
		case TextureFormat::R_8:
			tFlipVertically<u8>(info); break;
		case TextureFormat::RG_8:
			tFlipVertically<vector2<u8>>(info); break;
		case TextureFormat::RGB_8:
			tFlipVertically<vector3<u8>>(info); break;
		case TextureFormat::RGBA_8:
			tFlipVertically<vector4<u8>>(info); break;
		case TextureFormat::R_F32:
			tFlipVertically<float>(info); break;
		case TextureFormat::RG_F32:
			tFlipVertically<vec2>(info); break;
		case TextureFormat::RGB_F32:
			tFlipVertically<vec3>(info); break;
		case TextureFormat::RGBA_F32:
			tFlipVertically<vec4>(info); break;
		}
	}
}