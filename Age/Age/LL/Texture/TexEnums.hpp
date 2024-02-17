#pragma once

#include "Age/Math/vec2.hpp"
#include "Age/types.hpp"

namespace a_game_engine
{
	enum class TextureDataType
	{
		Ubyte, Float, Uint_24_8, Uint_24
	};
	enum class TextureFormat
	{
		Undefined = 0, R = 1, RG, RGB, RGBA,
		RGB_Float16, RGBA_Float16, RGB_Float32, RGBA_Float32,
		SRGB, SRGB_Alpha, 
		Depth16, Depth24, Depth32, Depth24_Stencil8,
		Auto, AutoSRGB
	};
	enum class TextureFiltering
	{
		Linear, Near,
		Linear_MipLinear, Linear_MipNear,
		Near_MipLinear, Near_MipNear
	};
	enum class TextureWrap { Repeat, MirroredRepeat, ClampToEdge };

	enum class MipmapSettings { Enable, Disable, Auto };

	class TexEnums
	{
	public:
		static int toOglFormat(TextureFormat);
		static void toOglOuterFormat(TextureFormat f, int& format, int& type);
		static TextureFormat chooseInternalFormat(TextureFormat imgFormat, TextureFormat texFormat);
		static TextureFiltering removeMipmaps(TextureFiltering);
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
		TextureFormat format = TextureFormat::Undefined;
		ubyte* data = nullptr;

		ImageInfo() = default;
		inline ImageInfo(const uvec2& size, TextureFormat format, ubyte* data = nullptr)
			: size(size), data(data), format(format) {}
		inline bool isValid() const {
			return 
				size.x != 0 and size.y != 0 and 
				data != nullptr and 
				format != TextureFormat::Undefined;
		}
	};
	struct Sampler2DInfo
	{
		TextureFiltering min, mag;
		TextureWrap wrapX, wrapY;

		inline Sampler2DInfo(TextureWrap x, TextureWrap y, TextureFiltering min, TextureFiltering mag)
			: wrapX(x), wrapY(y), min(min), mag(mag) {}

		inline Sampler2DInfo(TextureFiltering filtering = TextureFiltering::Linear_MipLinear, 
			TextureWrap wrap = TextureWrap::ClampToEdge)
			: min(filtering), mag(TexEnums::removeMipmaps(filtering)), wrapX(wrap), wrapY(wrap) {}
	};
	struct SamplerCubeInfo
	{
		TextureFiltering min, mag;
		TextureWrap wrapX, wrapY, wrapZ;

		inline SamplerCubeInfo(TextureWrap x, TextureWrap y, TextureWrap z, TextureFiltering min, TextureFiltering mag)
			: wrapX(x), wrapY(y), wrapZ(z), min(min), mag(mag) {}

		inline SamplerCubeInfo(TextureFiltering filtering = TextureFiltering::Linear_MipLinear, 
			TextureWrap wrap = TextureWrap::ClampToEdge)
			: min(filtering), mag(TexEnums::removeMipmaps(filtering)), wrapX(wrap), wrapY(wrap), wrapZ(wrap) {}
	};
}
