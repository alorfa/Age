#pragma once

#include "Age/LL/Texture/TexEnums.hpp"

namespace a_game_engine
{
	class Texture2D;

	class CubeMap
	{
		uint _id = 0;
		uint _size = 0;
	public:
		struct Settings
		{
			// right, left, front, back, top, bottom. Actially size means displacement
			const ImageInfo* images;
			uint size;
			TextureFormat format;
			SamplerCubeInfo sampler;
			int mipmaps;

			Settings(const ImageInfo* images, uint size,
				TextureFormat format = TextureFormat::SRGB, const SamplerCubeInfo& sampler = {},
				int mipmaps = -1);
		};
		struct PanoramaSettings
		{
			const Texture2D* panorama;
			uint size;
			TextureFormat format;
			SamplerCubeInfo sampler;
			int mipmaps;

			PanoramaSettings(const Texture2D& panorama, uint size = 0, TextureFormat format = TextureFormat::Auto,
				const SamplerCubeInfo& sampler = {}, int mipmaps = -1);
		};

		CubeMap();
		CubeMap(const CubeMap&) = delete;
		CubeMap& operator=(const CubeMap&) = delete;
		CubeMap(CubeMap&& other);
		CubeMap& operator=(CubeMap&& other);
		~CubeMap();
		void destroy();
		void create(const Settings& s);
		void createFromPanorama(const PanoramaSettings& s);
		void createSpecularMap(const CubeMap& cubemap);
		void createDiffuseMap(const CubeMap& cubemap);
		void activate(int number = 0) const;

		void setWrap(TextureWrap x, TextureWrap y, TextureWrap z);
		void setWrap(TextureWrap wrap);
		void setFiltering(TextureFiltering min, TextureFiltering mag);
		void setFiltering(TextureFiltering filter);
		void generateMipmaps();

		enum Face { X_Pos, X_Neg, Y_Pos, Y_Neg, Z_Pos, Z_Neg };

		inline uint getId() const { return _id; }
		inline uint getSize() const { return _size; }
	};

	struct EnvCubeMap
	{
		CubeMap specular, diffuse;
	};
}
