#include "TexEnums.hpp"
#include "Age/LL/opengl.h"
#include <cmath>
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
				info.swapRows<T>(y, info.size.y - 1 - y);
			}
		}
	}

	TextureFiltering TexEnums::removeMipmaps(TextureFiltering f)
	{
		switch (f)
		{
		case TextureFiltering::Linear:
		case TextureFiltering::Linear_MipLinear:
		case TextureFiltering::Linear_MipNear:
			return TextureFiltering::Linear;
		}
		return TextureFiltering::Near;
	}
	TextureFormat TexEnums::chooseInternalFormat(TextureFormat imgFormat, TextureFormat texFormat)
	{
		if (texFormat != TextureFormat::Auto &&
			texFormat != TextureFormat::AutoSRGB &&
			texFormat != TextureFormat::AutoQuality &&
			texFormat != TextureFormat::AutoSize)
			return texFormat;

		if (texFormat == TextureFormat::AutoSRGB)
		{
			int comps = TexEnums::getComponentsCount(imgFormat);
			if (comps == 4)
				return TextureFormat::S_RGBA;
			return TextureFormat::S_RGB;
		}
		if (texFormat == TextureFormat::AutoQuality)
		{
			return imgFormat;
		}
		if (texFormat == TextureFormat::Auto)
		{
			if (TexEnums::getSizeInBits(imgFormat) <= 16)
				return imgFormat;
			
			switch (TexEnums::getComponentsCount(imgFormat))
			{
			case 1:
				return TextureFormat::R_F16;
			case 2:
				return TextureFormat::RG_F16;
			case 3:
				return TextureFormat::RGB_F16;
			case 4:
				return TextureFormat::RGBA_F16;
			}
		}
		if (texFormat == TextureFormat::AutoSize)
		{
			if (TexEnums::getSizeInBits(imgFormat) <= 12)
				return imgFormat;
			
			switch (TexEnums::getComponentsCount(imgFormat))
			{
			case 1:
				return TextureFormat::R_F16;
			case 2:
				return TextureFormat::RG_F16;
			case 3:
				return TextureFormat::RGB_11_11_10;
			case 4:
				return TextureFormat::RGBA_F16;
			}
		}
		return imgFormat;
	}

    int TexEnums::toOglFormat(TextureFormat f)
    {
		switch (f)
		{
		case TextureFormat::R_8:
			return GL_R8;
		case TextureFormat::RG_8:
			return GL_RG8;
		case TextureFormat::RGB_8:
			return GL_RGB8;
		case TextureFormat::RGBA_8:
			return GL_RGBA8;

		case TextureFormat::R_F16:
			return GL_R16F;
		case TextureFormat::RG_F16:
			return GL_RG16F;
		case TextureFormat::RGB_F16:
			return GL_RGB16F;
		case TextureFormat::RGBA_F16:
			return GL_RGBA16F;

		case TextureFormat::R_F32:
			return GL_R32F;
		case TextureFormat::RG_F32:
			return GL_RG32F;
		case TextureFormat::RGB_F32:
			return GL_RGB32F;
		case TextureFormat::RGBA_F32:
			return GL_RGBA32F;

		case TextureFormat::S_R:
			return GL_SR8_EXT;
		case TextureFormat::S_RG:
			return GL_SRG8_EXT;
		case TextureFormat::S_RGB:
			return GL_SRGB8;
		case TextureFormat::S_RGBA:
			return GL_SRGB8_ALPHA8;

		case TextureFormat::RGB_11_11_10:
			return GL_R11F_G11F_B10F;

		case TextureFormat::Depth16:
			return GL_DEPTH_COMPONENT16;
		case TextureFormat::Depth24:
			return GL_DEPTH_COMPONENT24;
		case TextureFormat::Depth32:
			return GL_DEPTH_COMPONENT32F;
		case TextureFormat::Depth24_Stencil8:
			return GL_DEPTH24_STENCIL8;
		}
		return GL_RGB8;
    }

	void TexEnums::toOglOuterFormat(TextureFormat f, int& format, int& type)
	{
		switch (f)
		{
		case TextureFormat::R:
		case TextureFormat::RG:
		case TextureFormat::RGB:
		case TextureFormat::RGBA:
			type = GL_UNSIGNED_BYTE; 
			break;
		case TextureFormat::R_F16:
		case TextureFormat::RG_F16:
		case TextureFormat::RGB_F16:
		case TextureFormat::RGBA_F16:
			type = GL_HALF_FLOAT; 
			break;
		case TextureFormat::R_F32:
		case TextureFormat::RG_F32:
		case TextureFormat::RGB_F32:
		case TextureFormat::RGBA_F32:
			type = GL_FLOAT; 
			break;
		case TextureFormat::Depth24_Stencil8: //TODO: is it really need for depth formats?
			type = GL_UNSIGNED_INT_24_8;
			break;
		case TextureFormat::Depth16:
			type = GL_UNSIGNED_SHORT;
			break;
		default:
			type = GL_UNSIGNED_BYTE;
		}

		switch (f)
		{
		case TextureFormat::R:
		case TextureFormat::R_F16:
		case TextureFormat::R_F32:
		case TextureFormat::S_R:
			format = GL_RED;
			break;
		case TextureFormat::RG:
		case TextureFormat::RG_F16:
		case TextureFormat::RG_F32:
		case TextureFormat::S_RG:
			format = GL_RG;
			break;
		case TextureFormat::RGB:
		case TextureFormat::RGB_F16:
		case TextureFormat::RGB_F32:
		case TextureFormat::S_RGB:
			format = GL_RGB;
			break;
		case TextureFormat::RGBA:
		case TextureFormat::RGBA_F16:
		case TextureFormat::RGBA_F32:
		case TextureFormat::S_RGBA:
			format = GL_RGBA;
			break;
		case TextureFormat::Depth24_Stencil8: //TODO: is it necessary?
			format = GL_DEPTH_STENCIL;
			break;
		case TextureFormat::Depth16:
		case TextureFormat::Depth24:
		case TextureFormat::Depth32:
			format = GL_DEPTH_COMPONENT;
			break;
		default:
			format = GL_RGB;
		}
	}

    int TexEnums::toOglFilter(TextureFiltering f)
    {
		switch (f)
		{
		case TextureFiltering::Linear:
			return GL_LINEAR;
		case TextureFiltering::Near:
			return GL_NEAREST;
		case TextureFiltering::Linear_MipLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
		case TextureFiltering::Linear_MipNear:
			return GL_LINEAR_MIPMAP_NEAREST;
		case TextureFiltering::Near_MipLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case TextureFiltering::Near_MipNear:
			return GL_NEAREST_MIPMAP_NEAREST;
		}
		return GL_LINEAR;
    }

    int TexEnums::toOglWrap(TextureWrap w)
    {
		switch (w)
		{
		case TextureWrap::Repeat:
			return GL_REPEAT;
		case TextureWrap::MirroredRepeat:
			return GL_MIRRORED_REPEAT;
		case TextureWrap::ClampToEdge:
			return GL_CLAMP_TO_EDGE;
		}
		return GL_REPEAT;
    }

    int TexEnums::toOglType(TextureDataType t)
    {
		switch (t)
		{
		case TextureDataType::Float:
			return GL_FLOAT;
		case TextureDataType::Uint_24_8:
			return GL_UNSIGNED_INT_24_8;
		}
		return GL_UNSIGNED_BYTE;
    }

	int TexEnums::getComponentsCount(TextureFormat f)
	{
		switch (f)
		{
		case TextureFormat::R:
		case TextureFormat::R_F16:
		case TextureFormat::R_F32:
		case TextureFormat::S_R:
		case TextureFormat::Depth16:
		case TextureFormat::Depth24:
		case TextureFormat::Depth32:
			return 1;
		case TextureFormat::RG:
		case TextureFormat::RG_F16:
		case TextureFormat::RG_F32:
		case TextureFormat::S_RG:
		case TextureFormat::Depth24_Stencil8:
			return 2;
		case TextureFormat::RGB:
		case TextureFormat::RGB_F16:
		case TextureFormat::RGB_F32:
		case TextureFormat::S_RGB:
		case TextureFormat::RGB_11_11_10:
			return 3;
		case TextureFormat::RGBA:
		case TextureFormat::RGBA_F16:
		case TextureFormat::RGBA_F32:
		case TextureFormat::S_RGBA:
			return 4;
		}
		return 0;
	}
	int TexEnums::getSizeInBits(TextureFormat f)
	{
		switch (f)
		{
		case TextureFormat::R:
		case TextureFormat::RG:
		case TextureFormat::RGB:
		case TextureFormat::RGBA:
			return 8;
		case TextureFormat::R_F16:
		case TextureFormat::RG_F16:
		case TextureFormat::RGB_F16:
		case TextureFormat::RGBA_F16:
		case TextureFormat::Depth16:
			return 16;
		case TextureFormat::R_F32:
		case TextureFormat::RG_F32:
		case TextureFormat::RGB_F32:
		case TextureFormat::RGBA_F32:
		case TextureFormat::Depth32:
			return 32;
		case TextureFormat::RGB_11_11_10:
			return 11;
		case TextureFormat::Depth24:
			return 24;
		}
		return 8;
	}
	int TexEnums::computeMipLevels(uint maxSize)
	{
		return (int)std::floor(std::log2(maxSize)) + 1;
	}
	int TexEnums::computeMipLevels(uvec2 size)
	{
		return computeMipLevels(std::max(size.x, size.y));
	}
	void ImageInfo::flipVertically()
	{
		if (not isValid())
			return;

		switch (format)
		{
		case TextureFormat::R_8:
			tFlipVertically<u8>(*this); break;
		case TextureFormat::RG_8:
			tFlipVertically<vector2<u8>>(*this); break;
		case TextureFormat::RGB_8:
			tFlipVertically<vector3<u8>>(*this); break;
		case TextureFormat::RGBA_8:
			tFlipVertically<vector4<u8>>(*this); break;
		case TextureFormat::R_F32:
			tFlipVertically<float>(*this); break;
		case TextureFormat::RG_F32:
			tFlipVertically<vec2>(*this); break;
		case TextureFormat::RGB_F32:
			tFlipVertically<vec3>(*this); break;
		case TextureFormat::RGBA_F32:
			tFlipVertically<vec4>(*this); break;
		}
	}
}
