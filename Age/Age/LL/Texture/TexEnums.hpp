#pragma once

#include "Age/Math/vec2.hpp"
#include "Age/types.hpp"

namespace a_game_engine
{
	enum class TextureDataType
	{
		Ubyte, Float, Uint_24_8
	};
	enum class TextureFormat
	{
		Undefined = 0, R = 1, RG, RGB, RGBA,
		RGB_Float16, RGBA_Float16, RGB_Float32, RGBA_Float32,
		SRGB, SRGB_Alpha, Depth24_Stencil8
	};
	enum class TextureFiltering
	{
		Linear, Near,
		LinearMipLinear, LinearMipNear,
		NearMipLinear, NearMipNear
	};
	enum class TextureWrap { Repeat, MirroredRepeat, ClampToEdge };

	class TexEnums
	{
	public:
		static int toOglFormat(TextureFormat);
		static void toOglOuterFormat(TextureFormat f, int& format, int& type);
		static int toOglFilter(TextureFiltering);
		static int toOglWrap(TextureWrap);
		static int toOglType(TextureDataType);
		static TextureFormat toSRGB(TextureFormat);
		static int getComponentsCount(TextureFormat);
		static int computeMipLevels(uint maxSize);
		static int computeMipLevels(uvec2 size);
	};

	struct ImageInfo
	{
		uvec2 size;
		ubyte* data = nullptr;
		TextureFormat format = TextureFormat::Undefined;

		ImageInfo() = default;
		inline ImageInfo(const uvec2& size, ubyte* data, TextureFormat format)
			: size(size), data(data), format(format) {}
		inline bool isValid() const {
			return 
				size.x != 0 and size.y != 0 and 
				data != nullptr and 
				format != TextureFormat::Undefined;
		}
	};
}
