#include "CubeMap.hpp"
#include "Age/LL/opengl.h"
#include <utility>
#include <cmath>
#include <algorithm>

namespace a_game_engine
{
	CubeMap::CubeMap()
	{
	}
	CubeMap::CubeMap(CubeMap&& other)
		: _id(other._id)
	{
		other._id = 0;
	}
	CubeMap& CubeMap::operator=(CubeMap&& other)
	{
		std::swap(_id, other._id);
		return *this;
	}
	CubeMap::~CubeMap()
	{
		if (_id)
			glDeleteTextures(1, &_id);
	}
	void CubeMap::create(const Settings& s)
	{
		if (_id == 0)
			glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_id);

		const int maxMipLevels = (int)std::floor(std::log2(std::max(s.imagesArea.x, s.imagesArea.y))) + 1;
		const int mipLevels = s.mipmaps ? maxMipLevels : 1;
		glTextureStorage2D(_id, mipLevels, TexEnums::toOglFormat(s.internal), s.imagesArea.x, s.imagesArea.y);
		//glTextureStorage3D(_id, mipLevels, TexEnums::toOglFormat(s.internal), s.imagesArea.x, s.imagesArea.y, 1);

		bool hasData = false;
		for (uint i = 0; i < 6; i++)
		{
			const auto& img = s.images[i];
			if (img.data)
			{
				int outerType, outerFormat;
				TexEnums::toOglOuterFormat(img.format, outerFormat, outerType);
				glTextureSubImage3D(_id, 0,
					img.size.x, img.size.y, i, 
					s.imagesArea.x, s.imagesArea.y, 1,
					outerFormat, outerType, img.data);
				hasData = true;
			}
		}
		if (hasData)
			generateMipmaps();
	}
	void CubeMap::generateMipmaps()
	{
		glGenerateTextureMipmap(_id);
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
		glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, TexEnums::toOglFilter(mag));
	}
	void CubeMap::setFiltering(TextureFiltering filter)
	{
		setFiltering(filter, filter);
	}
}
