#include "Texture2D.hpp"
#include "Age/LL/opengl.h"
#include <utility>

namespace a_game_engine
{
	int Texture2D::toOglFormat(TextureFormat f)
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
		case TextureFormat::Depth24_Stencil8:
			return GL_DEPTH24_STENCIL8;
		}
		return GL_RGBA;
	}
	int Texture2D::toOglFilter(TextureFiltering f)
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
	int Texture2D::toOglWrap(TextureWrap w)
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

	int Texture2D::toOglType(TextureDataType t)
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

	Texture2D::Texture2D(const Settings& s)
	{
		create(s);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &_id);
	}

	Texture2D::Texture2D(Texture2D&& other) noexcept
	{
		operator=(std::move(other));
	}

	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
	{
		std::swap(other._id, _id);
		std::swap(other._size, _size);
		return *this;
	}

	void Texture2D::create(const Settings& s)
	{
		if (_id)
			glDeleteTextures(1, &_id);

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);

		_size = s.size;

		int outerFormat = s.internal == TextureFormat::Depth24_Stencil8 ?
			GL_DEPTH_STENCIL : toOglFormat(s.outer);
		glTexImage2D(GL_TEXTURE_2D, 0, toOglFormat(s.internal),
			s.size.x, s.size.y, 0,
			outerFormat, toOglType(s.type), s.data);
		if (s.mipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);
	}

	void Texture2D::setWrap(TextureWrap x, TextureWrap y)
	{
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toOglWrap(x)); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toOglWrap(y));
	}
	void Texture2D::setWrap(TextureWrap wrap)
	{
		setWrap(wrap, wrap);
	}
	void Texture2D::setFiltering(TextureFiltering min, TextureFiltering mag)
	{
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toOglFilter(min));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toOglFilter(mag));
	}
	void Texture2D::setFiltering(TextureFiltering filter)
	{
		setFiltering(filter, filter);
	}
	void Texture2D::activate(int number) const
	{
		glActiveTexture(GL_TEXTURE0 + number);
		glBindTexture(GL_TEXTURE_2D, _id);
	}
	Texture2D::Settings::Settings(const uvec2& size, const ubyte* data, TextureFormat internal, 
		TextureFormat outer, TextureDataType type, bool mipmaps)
		: size(size), data(data), internal(internal), outer(outer), type(type), mipmaps(mipmaps)
	{}
	Texture2D::Settings::Settings(const uvec2& size, const ubyte* data, TextureFormat format, 
		TextureDataType type, bool mipmaps)
		: size(size), data(data), internal(format), outer(format), type(type), mipmaps(mipmaps)
	{}
}