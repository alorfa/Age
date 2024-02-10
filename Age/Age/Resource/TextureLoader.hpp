#pragma once

#include <map>
#include "Age/LL/Texture/Texture2D.hpp"
#include "Age/LL/Texture/CubeMap.hpp"
#include "Image.hpp"

namespace a_game_engine
{
	class TextureLoader
	{
		static std::unique_ptr<Image> defaultImage;

		static std::unique_ptr<Texture2D> defaultTexture;
		std::map<std::filesystem::path, std::unique_ptr<Texture2D>> textures;

		static std::unique_ptr<CubeMap> defCubemap;
		std::map<std::wstring, std::unique_ptr<CubeMap>> cubeMaps;
		std::map<std::filesystem::path, std::unique_ptr<CubeMap>> newCubeMaps;
	public:
		static std::filesystem::path defaultImagePath;

		bool logOnLoad = true;
		bool logOnUnload = true;
		~TextureLoader();

		struct Settings
		{
			TextureFiltering 
				minFilter = TextureFiltering::LinearMipLinear, 
				magFilter = TextureFiltering::Linear;
			TextureWrap 
				wrapX = TextureWrap::Repeat,  
				wrapY = TextureWrap::Repeat;
			bool srgb = false;

			inline Settings(
				TextureFiltering min = TextureFiltering::LinearMipLinear,
				TextureFiltering mag = TextureFiltering::Linear,
				TextureWrap wrapX = TextureWrap::Repeat,
				TextureWrap wrapY = TextureWrap::Repeat,
				bool srgb = false)
				: minFilter(min), magFilter(mag), wrapX(wrapX), wrapY(wrapY), srgb(srgb) {}
		};
		struct CubemapSettings
		{
			TextureFiltering
				minFilter = TextureFiltering::Linear,
				magFilter = TextureFiltering::Linear;
			TextureWrap
				wrapX = TextureWrap::ClampToEdge,
				wrapY = TextureWrap::ClampToEdge,
				wrapZ = TextureWrap::ClampToEdge;
			TextureFormat internalFormat = TextureFormat::SRGB;
			int faceSize = -1;

			inline CubemapSettings(TextureFiltering filter = TextureFiltering::Linear,
				TextureWrap wrap = TextureWrap::ClampToEdge, TextureFormat format = TextureFormat::SRGB, 
				int faceSize = -1)
					: minFilter(filter), magFilter(filter), wrapX(wrap), wrapY(wrap), wrapZ(wrap), 
				internalFormat(format), faceSize(faceSize) {}
			inline CubemapSettings(
				TextureFiltering min, TextureFiltering mag,
				TextureWrap wrapX = TextureWrap::ClampToEdge, 
				TextureWrap wrapY = TextureWrap::ClampToEdge, 
				TextureWrap wrapZ = TextureWrap::ClampToEdge, 
				TextureFormat format = TextureFormat::SRGB, int faceSize = -1)
				: faceSize(faceSize), minFilter(min), magFilter(mag),
				wrapX(wrapX), wrapY(wrapY), wrapZ(wrapZ), internalFormat(format) {}
		};

		Texture2D& load(const std::filesystem::path& path, const Settings& s = Settings());

		// +X, -X, +Y, -Y, +Z, -Z
		CubeMap& loadCubeMap(const std::filesystem::path* paths, const CubemapSettings& s = CubemapSettings());
		CubeMap& loadCubeMap(const std::filesystem::path& path, const CubemapSettings& s = CubemapSettings());

		static Texture2D& getDefault();
		static CubeMap& getDefaultCubeMap();
		static Image& getDefaultImage();
	};
}