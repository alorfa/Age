#include "CubeMap.hpp"
#include "Age/LL/opengl.h"
#include <utility>

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
		if (_id)
			glDeleteTextures(1, &_id);

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

		for (uint i = 0; i < 6; i++)
		{
			const auto& img = s.images[i];
			int outerFormat, outerType;
			int innerFormat = TexEnums::toOglFormat(s.internal);
			TexEnums::toOglOuterFormat(img.format, outerFormat, outerType);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				innerFormat, img.size.x, img.size.y, 0, outerFormat, outerType, img.data);
		}
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
