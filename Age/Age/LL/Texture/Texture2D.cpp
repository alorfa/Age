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

		glCreateTextures(GL_TEXTURE_2D, 1, &_id);

		_size = s.img.size;

		int outerType, outerFormat;
		int innerFormat = TexEnums::toOglFormat(s.internal);
		TexEnums::toOglOuterFormat(s.img.format, outerFormat, outerType);
		glTextureStorage2D(_id, 1, TexEnums::toOglFormat(s.internal), _size.x, _size.y);
		glTextureSubImage2D(_id, 0, 0, 0, _size.x, _size.y, outerFormat, outerType, s.img.data);
		//if (s.mipmaps)
		//	glGenerateTextureMipmap(_id);
	}

	void Texture2D::setWrap(TextureWrap x, TextureWrap y)
	{
		glTextureParameteri(_id, GL_TEXTURE_WRAP_S, TexEnums::toOglWrap(x));
		glTextureParameteri(_id, GL_TEXTURE_WRAP_T, TexEnums::toOglWrap(y));
	}
	void Texture2D::setWrap(TextureWrap wrap)
	{
		setWrap(wrap, wrap);
	}
	void Texture2D::setFiltering(TextureFiltering min, TextureFiltering mag)
	{
		glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, TexEnums::toOglFilter(min));
		glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, TexEnums::toOglFilter(mag));
	}
	void Texture2D::setFiltering(TextureFiltering filter)
	{
		setFiltering(filter, filter);
	}
	void Texture2D::activate(int number) const
	{
		glBindTextureUnit(number, _id);
	}

	Texture2D::Settings::Settings(const ImageInfo& img, TextureFormat internal, bool mipmaps)
		: img(img), internal(internal), mipmaps(mipmaps)
	{}
	Texture2D::Settings::Settings(const ImageInfo& img, bool mipmaps)
		: img(img), internal(img.format), mipmaps(mipmaps)
	{}
}