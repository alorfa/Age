#include "TexEnums.hpp"
#include "Age/LL/opengl.h"
#include <cmath>

namespace a_game_engine
{
    int TexEnums::toOglFormat(TextureFormat f)
    {
		switch (f)
		{
		case TextureFormat::R:
			return GL_R8;
		case TextureFormat::RG:
			return GL_RG8;
		case TextureFormat::RGB:
			return GL_RGB8;
		case TextureFormat::RGBA:
			return GL_RGBA8;
		case TextureFormat::RGB_Float16:
			return GL_RGB16F;
		case TextureFormat::RGBA_Float16:
			return GL_RGBA16F;
		case TextureFormat::RGB_Float32:
			return GL_RGB32F;
		case TextureFormat::RGBA_Float32:
			return GL_RGBA32F;
		case TextureFormat::SRGB:
			return GL_SRGB8;
		case TextureFormat::SRGB_Alpha:
			return GL_SRGB8_ALPHA8;
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
		case TextureFormat::RGB_Float16:
		case TextureFormat::RGBA_Float16:
			type = GL_HALF_FLOAT; 
			break;
		case TextureFormat::RGB_Float32:
		case TextureFormat::RGBA_Float32:
			type = GL_FLOAT; 
			break;
		case TextureFormat::Depth24_Stencil8:
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
		case TextureFormat::RGB:
		case TextureFormat::SRGB:
		case TextureFormat::RGB_Float16:
		case TextureFormat::RGB_Float32:
			format = GL_RGB;
			break;
		case TextureFormat::RGBA:
		case TextureFormat::SRGB_Alpha:
		case TextureFormat::RGBA_Float16:
		case TextureFormat::RGBA_Float32:
			format = GL_RGBA;
			break;
		case TextureFormat::Depth24_Stencil8:
			format = GL_DEPTH_STENCIL;
			break;
		case TextureFormat::Depth16:
		case TextureFormat::Depth24:
		case TextureFormat::Depth32:
			format = GL_DEPTH_COMPONENT;
			break;
		case TextureFormat::R:
			format = GL_RED;
			break;
		case TextureFormat::RG:
			format = GL_RG;
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
		case TextureFiltering::LinearMipLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
		case TextureFiltering::LinearMipNear:
			return GL_LINEAR_MIPMAP_NEAREST;
		case TextureFiltering::NearMipLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case TextureFiltering::NearMipNear:
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
	TextureFormat TexEnums::toSRGB(TextureFormat f)
	{
		int comps = getComponentsCount(f);
		if (comps == 4)
			return TextureFormat::SRGB_Alpha;
		return TextureFormat::SRGB;
	}
	int TexEnums::getComponentsCount(TextureFormat f)
	{
		switch (f)
		{
		case TextureFormat::R:
			return 1;
		case TextureFormat::RG:
		case TextureFormat::Depth24_Stencil8:
			return 2;
		case TextureFormat::SRGB:
		case TextureFormat::RGB:
		case TextureFormat::RGB_Float16:
		case TextureFormat::RGB_Float32:
			return 3;
		case TextureFormat::SRGB_Alpha:
		case TextureFormat::RGBA:
		case TextureFormat::RGBA_Float16:
		case TextureFormat::RGBA_Float32:
			return 4;
		}
		return 0;
	}
	int TexEnums::computeMipLevels(uint maxSize)
	{
		return (int)std::floor(std::log2(maxSize)) + 1;
	}
	int TexEnums::computeMipLevels(uvec2 size)
	{
		return computeMipLevels(std::max(size.x, size.y));
	}
}
