#pragma once

#include "Age/LL/Texture/Texture2D.hpp"
#include "Age/LL/Texture/Image.hpp"
#include "Age/LL/Texture/CubeMap.hpp"
#include <map>

namespace a_game_engine
{
	class TextureLoader
	{
		static std::unique_ptr<Image> defaultImage;

		static std::unique_ptr<Texture2D> defaultTexture;
		std::map<std::filesystem::path, std::unique_ptr<Texture2D>> textures;

		static std::unique_ptr<CubeMap> defCubemap;
		std::map<std::wstring, std::unique_ptr<CubeMap>> cubeMaps;
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
		};
		struct CubemapSettings
		{
			int faceSize = -1;
			TextureFiltering
				minFilter = TextureFiltering::Linear,
				magFilter = TextureFiltering::Linear;
			TextureWrap
				wrapX = TextureWrap::ClampToEdge,
				wrapY = TextureWrap::ClampToEdge,
				wrapZ = TextureWrap::ClampToEdge;
			TextureFormat internalFormat = TextureFormat::SRGB;
		};

		Texture2D& load(const std::filesystem::path& path, const Settings& s = Settings());

		// +X, -X, +Y, -Y, +Z, -Z
		CubeMap& loadCubeMap(const std::filesystem::path* paths, const CubemapSettings& s = CubemapSettings());

		static Texture2D& getDefault();
		static CubeMap& getDefaultCubeMap();
		static Image& getDefaultImage();
	};
}