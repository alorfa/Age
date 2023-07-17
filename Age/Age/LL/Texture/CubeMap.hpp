#pragma once

#include "Age/LL/Texture/TexEnums.hpp"

namespace a_game_engine
{
	class CubeMap
	{
		uint _id = 0;
	public:
		struct Settings
		{
			// right, left, front, back, top, bottom
			const ImageInfo* images;
			TextureFormat internal;
			bool mipmaps = true;
		};

		CubeMap();
		CubeMap(const CubeMap&) = delete;
		CubeMap& operator=(const CubeMap&) = delete;
		CubeMap(CubeMap&& other);
		CubeMap& operator=(CubeMap&& other);
		~CubeMap();
		void create(const Settings& s);

		void setWrap(TextureWrap x, TextureWrap y, TextureWrap z);
		void setWrap(TextureWrap wrap);
		void setFiltering(TextureFiltering min, TextureFiltering mag);
		void setFiltering(TextureFiltering filter);
	};
}
