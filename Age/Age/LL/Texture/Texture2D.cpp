#include "Texture2D.hpp"
#include "Age/LL/opengl.h"
#include <utility>

namespace a_game_engine
{
	Texture2D::Texture2D(const Settings& s)
	{
		create(s);
	}

	Texture2D::~Texture2D()
	{
		if (_id)
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

		_size = s.img.size;

		int outerFormat = s.internal == TextureFormat::Depth24_Stencil8 ?
			GL_DEPTH_STENCIL : TexEnums::toOglFormat(s.img.format);
		glTexImage2D(GL_TEXTURE_2D, 0, TexEnums::toOglFormat(s.internal),
			_size.x, _size.y, 0,
			outerFormat, TexEnums::toOglType(s.img.type), s.img.data);
		if (s.mipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);
	}

	void Texture2D::setWrap(TextureWrap x, TextureWrap y)
	{
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TexEnums::toOglWrap(x));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TexEnums::toOglWrap(y));
	}
	void Texture2D::setWrap(TextureWrap wrap)
	{
		setWrap(wrap, wrap);
	}
	void Texture2D::setFiltering(TextureFiltering min, TextureFiltering mag)
	{
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TexEnums::toOglFilter(min));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TexEnums::toOglFilter(mag));
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

	Texture2D::Settings::Settings(const ImageInfo& img, TextureFormat internal, bool mipmaps)
		: img(img), internal(internal), mipmaps(mipmaps)
	{}
	Texture2D::Settings::Settings(const ImageInfo& img, bool mipmaps)
		: img(img), internal(img.format), mipmaps(mipmaps)
	{}
}