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
		Undefined = 0, 
		R_8 = 1, RG_8, RGB_8, RGBA_8,
		R_F16, RG_F16, RGB_F16, RGBA_F16, 
		R_F32, RG_F32, RGB_F32, RGBA_F32,
		S_R, S_RG, S_RGB, S_RGBA,
		RGB_11_11_10,
		Depth16, Depth24, Depth32, Depth24_Stencil8,
		Auto, AutoSRGB, AutoQuality, AutoSize,
		R = R_8, RG = RG_8, RGB = RGB_8, RGBA = RGBA_8
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
		static TextureFormat chooseInternalFormat(TextureFormat imgFormat, TextureFormat texFormat);
		static TextureFiltering removeMipmaps(TextureFiltering);
		static int toOglFormat(TextureFormat);
		static void toOglOuterFormat(TextureFormat f, int& format, int& type);
		static int toOglFilter(TextureFiltering);
		static int toOglWrap(TextureWrap);
		static int toOglType(TextureDataType);
		static int getComponentsCount(TextureFormat);
		static int getSizeInBits(TextureFormat);
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

		template <typename T>
		T& changePixel(uvec2 pos)
		{
			T* startData = (T*)data;
			if (startData == nullptr)
				return *startData;

			return startData[size.x * pos.y + pos.x];
		}
		template <typename T>
		void swapRows(uint y1, uint y2)
		{
			for (uint i = 0; i < size.x; i++)
			{
				T& v1 = changePixel<T>({ i, y1 });
				T& v2 = changePixel<T>({ i, y2 });
				T temp = v1;
				v1 = v2;
				v2 = temp;
			}
		}
		void flipVertically();
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
