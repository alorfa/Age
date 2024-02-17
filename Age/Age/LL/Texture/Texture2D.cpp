#include "Texture2D.hpp"
#include "Age/LL/opengl.h"
#include <utility>
#include <algorithm>
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	Texture2D::Texture2D(const Settings& s)
	{
		create(s);
	}

	Texture2D::~Texture2D()
	{
		destroy();
	}

	Texture2D::Texture2D(Texture2D&& other) noexcept
	{
		operator=(std::move(other));
	}

	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
	{
		std::swap(other._id, _id);
		std::swap(other._size, _size);
		std::swap(other._format, _format);
		return *this;
	}

	void Texture2D::create(const Settings& s)
	{
		destroy();
		glCreateTextures(GL_TEXTURE_2D, 1, &_id);

		_size = s.img.size;
		_format = s.format;

		int outerType, outerFormat;
		const int innerFormat = TexEnums::toOglFormat(_format);
		TexEnums::toOglOuterFormat(s.img.format, outerFormat, outerType);
		glTextureStorage2D(_id, s.mipmaps, innerFormat, _size.x, _size.y);
		if (s.img.data)
			glTextureSubImage2D(_id, 0, 0, 0, _size.x, _size.y, outerFormat, outerType, s.img.data);
		if (s.mipmaps > 1 and s.img.data)
			generateMipmaps();

		setWrap(s.sampler.wrapX, s.sampler.wrapY);
		setFiltering(s.sampler.min, s.sampler.mag);
	}

	void Texture2D::generateMipmaps()
	{
		glGenerateTextureMipmap(_id);
	}

	void Texture2D::destroy()
	{
		if (_id)
		{
			glDeleteTextures(1, &_id);
			_id = 0;
		}
		_size = { 0, 0 };
		_format = TextureFormat::Undefined;
	}

	vec3 Texture2D::getMidColor() const
	{
		vec3 result;
		if (_id)
			glGetTextureImage(_id, TexEnums::computeMipLevels(_size) - 1, GL_RGB, GL_FLOAT, sizeof(vec3), &result);

		return result;
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
		glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, TexEnums::toOglFilter(TexEnums::removeMipmaps(mag)));
	}
	void Texture2D::setFiltering(TextureFiltering filter)
	{
		setFiltering(filter, filter);
	}
	void Texture2D::activate(int number) const
	{
		glBindTextureUnit(number, _id);
	}

	Texture2D::Settings::Settings(const ImageInfo& img, TextureFormat format, const Sampler2DInfo& sampler, int mipmaps)
		: img(img), sampler(sampler)
	{
		const int maxMips = TexEnums::computeMipLevels(img.size);
		if (mipmaps < 1)
			this->mipmaps = maxMips;
		else
			this->mipmaps = Math::min(mipmaps, maxMips);

		this->format = TexEnums::chooseInternalFormat(img.format, format);
	}
}