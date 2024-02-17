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
		std::map<std::filesystem::path, std::unique_ptr<CubeMap>> newCubeMaps;

		static std::unique_ptr<EnvCubeMap> defEnvCubemap;
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
			TextureFormat format;
			uint size;
			MipmapSettings mipmaps;

			inline CubeMapSettings(const SamplerCubeInfo& sampler = SamplerCubeInfo{},
				TextureFormat format = TextureFormat::SRGB, uint size = 0,
				MipmapSettings mipmaps = MipmapSettings::Auto)
					: sampler(sampler), format(format), size(size), mipmaps(mipmaps) {}

			bool hasMipmaps() const;
		};
		struct EnvCubeMapSettings
		{
			TextureFormat internalFormat;
			uint size;
			uint diffuseSize;

			inline EnvCubeMapSettings(TextureFormat format = TextureFormat::SRGB, uint diffuseSize = 32,
				uint size = 0)
				: internalFormat(format), diffuseSize(diffuseSize),
				size(size) {}
		};

		Texture2D& load(const std::filesystem::path& path, const Settings& s = {});
		CubeMap& loadCubeMap(const std::filesystem::path& path, const CubeMapSettings& s = {});
		void removeCubeMap(const std::filesystem::path& path);
		static std::unique_ptr<CubeMap> readCubeMapFromFile(const std::filesystem::path& path, const CubeMapSettings& s = {});
		EnvCubeMap& loadEnv(const std::filesystem::path& path, const CubeMapSettings& s = {});

		static Texture2D& getDefault();
		static CubeMap& getDefaultCubeMap();
		static EnvCubeMap& getDefaultEnvCubeMap();
		static Image& getDefaultImage();
	};
}