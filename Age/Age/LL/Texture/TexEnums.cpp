#include "TexEnums.hpp"
#include "Age/LL/opengl.h"

namespace a_game_engine
{
    int TexEnums::toOglFormat(TextureFormat f)
    {
		switch (f)
		{
		case TextureFormat::R:
			return GL_RED;
		case TextureFormat::RG:
			return GL_RG;
		case TextureFormat::RGB:
			return GL_RGB;
		case TextureFormat::RGBA:
			return GL_RGBA;
		case TextureFormat::RGB_Float16:
			return GL_RGB16F;
		case TextureFormat::RGBA_Float16:
			return GL_RGBA16F;
		case TextureFormat::RGB_Float32:
			return GL_RGB32F;
		case TextureFormat::RGBA_Float32:
			return GL_RGBA32F;
		case TextureFormat::SRGB:
			return GL_SRGB;
		case TextureFormat::SRGB_Alpha:
			return GL_SRGB_ALPHA;
		case TextureFormat::Depth24_Stencil8:
			return GL_DEPTH24_STENCIL8;
		}
		return GL_RGBA;
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
}
