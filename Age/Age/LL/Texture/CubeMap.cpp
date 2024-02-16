#include "CubeMap.hpp"
#include "Age/LL/opengl.h"
#include <utility>
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	CubeMap::CubeMap()
	{
	}
	CubeMap::CubeMap(CubeMap&& other)
	{
		operator=(std::move(other));
	}
	CubeMap& CubeMap::operator=(CubeMap&& other)
	{
		std::swap(_id, other._id);
		std::swap(size, other.size);
		return *this;
	}
	CubeMap::~CubeMap()
	{
		destroy();
	}
	void CubeMap::destroy()
	{
		if (_id)
		{
			glDeleteTextures(1, &_id);
			_id = 0;
		}
	}
	void CubeMap::create(const Settings& s)
	{
		destroy();
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_id);

		size = s.imageArea;

		glTextureStorage2D(_id, s.mipmaps, TexEnums::toOglFormat(s.format), size, size);
		setWrap(s.sampler.wrapX, s.sampler.wrapY, s.sampler.wrapZ);
		setFiltering(s.sampler.min, s.sampler.mag);
		if (s.images == nullptr)
			return;

		bool hasData = false;
		for (uint i = 0; i < 6; i++)
		{
			const auto& img = s.images[i];
			if (img.data)
			{
				hasData = true;
				int outerType, outerFormat;
				TexEnums::toOglOuterFormat(img.format, outerFormat, outerType);
				if (img.data)
					glTextureSubImage3D(_id, 0, 0, 0, i, s.imageArea, s.imageArea, 1,
						outerFormat, outerType, img.data);
			}
		}
		if (s.mipmaps > 1 && hasData)
			generateMipmaps();
	}
	void CubeMap::activate(int number) const
	{
		glBindTextureUnit(number, _id);
	}
	void CubeMap::setWrap(TextureWrap x, TextureWrap y, TextureWrap z)
	{
		glTextureParameteri(_id, GL_TEXTURE_WRAP_S, TexEnums::toOglWrap(x));
		glTextureParameteri(_id, GL_TEXTURE_WRAP_T, TexEnums::toOglWrap(y));
		glTextureParameteri(_id, GL_TEXTURE_WRAP_R, TexEnums::toOglWrap(z));
	}
	void CubeMap::setWrap(TextureWrap wrap)
	{
		setWrap(wrap, wrap, wrap);
	}
	void CubeMap::setFiltering(TextureFiltering min, TextureFiltering mag)
	{
		glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, TexEnums::toOglFilter(min));
		glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, TexEnums::toOglFilter(TexEnums::removeMipmaps(mag)));
	}
	void CubeMap::setFiltering(TextureFiltering filter)
	{
		setFiltering(filter, filter);
	}
	void CubeMap::generateMipmaps()
	{
		glGenerateTextureMipmap(_id);
	}

	CubeMap::Settings::Settings(const ImageInfo* images, uint imageArea, TextureFormat format,
		const SamplerCubeInfo& sampler, int mipmaps)
		: images(images), imageArea(imageArea), format(format), sampler(sampler)
	{
		const int maxMips = TexEnums::computeMipLevels(imageArea);
		if (mipmaps < 1)
			this->mipmaps = maxMips;
		else
			this->mipmaps = Math::min(mipmaps, maxMips);
	}
}
