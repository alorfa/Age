#pragma once

#include "Age/LL/Texture/TexEnums.hpp"

namespace a_game_engine
{
	class CubeMap
	{
		uint _id = 0;
		uint size = 0;
	public:
		struct Settings
		{
			// right, left, front, back, top, bottom. Actially size means displacement
			const ImageInfo* images;
			TextureFormat internal;
			uint imageArea;
			bool mipmaps = true;

			inline Settings(const ImageInfo* images, uint imageArea,
				TextureFormat internalFormat = TextureFormat::SRGB, bool mipmaps = true)
				: images(images), imageArea(imageArea), internal(internalFormat), mipmaps(mipmaps) {}
		};

		CubeMap();
		CubeMap(const CubeMap&) = delete;
		CubeMap& operator=(const CubeMap&) = delete;
		CubeMap(CubeMap&& other);
		CubeMap& operator=(CubeMap&& other);
		~CubeMap();
		void destroy();
		void create(const Settings& s);
		void activate(int number = 0) const;

		void setWrap(TextureWrap x, TextureWrap y, TextureWrap z);
		void setWrap(TextureWrap wrap);
		void setFiltering(TextureFiltering min, TextureFiltering mag);
		void setFiltering(TextureFiltering filter);
		void generateMipmaps();

		enum Face { X_Pos, X_Neg, Y_Pos, Y_Neg, Z_Pos, Z_Neg };

		inline uint getId() const { return _id; }
		inline uint getSize() const { return size; }
	};
}
