#pragma once

#include <map>
#include "Age/LL/Texture/Texture2D.hpp"
#include "Age/LL/Texture/CubeMap.hpp"
#include "Image.hpp"

namespace a_game_engine
{
	class TextureLoader
	{
	public:
		struct EnvCubeMap
		{
			CubeMap specular, diffuse;
		};
	private:
		static std::unique_ptr<Image> defaultImage;

		static std::unique_ptr<Texture2D> defaultTexture;
		std::map<std::filesystem::path, std::unique_ptr<Texture2D>> textures;

		static std::unique_ptr<CubeMap> defCubemap;
		std::map<std::filesystem::path, std::unique_ptr<CubeMap>> newCubeMaps;

		std::map<std::filesystem::path, std::unique_ptr<EnvCubeMap>> cubeMaps;
	public:
		static std::filesystem::path defaultImagePath;

		bool logOnLoad = true;
		bool logOnUnload = true;
		~TextureLoader();

		struct Settings
		{
			Sampler2DInfo sampler;
			TextureFormat format;
			MipmapSettings mipmaps;

			Settings(
				const Sampler2DInfo& sampler = Sampler2DInfo{},
				TextureFormat format = TextureFormat::AutoSRGB,
				MipmapSettings mipmaps = MipmapSettings::Auto);

			bool hasMipmaps() const;
		};
		struct CubeMapSettings
		{
			SamplerCubeInfo sampler;
			TextureFormat internalFormat;
			int resolution;
			int diffuseResolution;
			MipmapSettings mipmaps;

			inline CubeMapSettings(const SamplerCubeInfo& sampler = SamplerCubeInfo{},
				TextureFormat format = TextureFormat::SRGB, int diffuseResolution = 32, int resolution = -1, 
				MipmapSettings mipmaps = MipmapSettings::Auto)
					: sampler(sampler), internalFormat(format), diffuseResolution(diffuseResolution), 
				resolution(resolution), mipmaps(mipmaps) {}

			bool hasMipmaps() const;
		};

		Texture2D& load(const std::filesystem::path& path, const Settings& s = {});
		CubeMap& loadCubeMap(const std::filesystem::path& path, const CubeMapSettings& s = {});
		void removeCubeMap(const std::filesystem::path& path);
		//EnvCubeMap& loadEnv(const std::filesystem::path& path, const CubeMapSettings& s = {});

		static Texture2D& getDefault();
		static CubeMap& getDefaultCubeMap();
		static Image& getDefaultImage();
	};
}