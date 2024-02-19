#pragma once

#include "Age/LL/Texture/CubeMap.hpp"
#include <map>
#include <filesystem>

namespace a_game_engine
{
	class CubeMapLoader
	{
		static std::unique_ptr<CubeMap> defCubemap;
		std::map<std::filesystem::path, std::unique_ptr<CubeMap>> rawCubeMaps;

		static std::unique_ptr<EnvCubeMap> defEnvCubemap;
		std::map<std::filesystem::path, std::unique_ptr<EnvCubeMap>> cubeMaps;
	public:
		struct RawSettings
		{
			SamplerCubeInfo sampler;
			TextureFormat format;
			uint size;
			MipmapSettings mipmaps;

			inline RawSettings(const SamplerCubeInfo& sampler = SamplerCubeInfo{},
				TextureFormat format = TextureFormat::SRGB, uint size = 0,
				MipmapSettings mipmaps = MipmapSettings::Auto)
				: sampler(sampler), format(format), size(size), mipmaps(mipmaps) {}

			bool hasMipmaps() const;
		};
		struct Settings
		{
			TextureFormat specularFormat, diffuseFormat, tempFormat;
			bool srgb = true;
			uint specularSize;
			uint diffuseSize;

			Settings(TextureFormat specularFormat, TextureFormat diffuseFormat, TextureFormat tempFormat,
				bool srgb = true, uint specularSize = 0, uint diffuseSize = 32);
			//Settings(TextureFormat format, bool srgb = true, 
			//	uint specularSize = 0, uint diffuseSize = 32);
		};

		static std::unique_ptr<CubeMap> readFromFile(const std::filesystem::path& path, const RawSettings& s = {});
		const CubeMap& loadRaw(const std::filesystem::path& path, const RawSettings& s = {});
		const EnvCubeMap& load(const std::filesystem::path& path, const Settings& s);

		static CubeMap& getDefaultCubeMap();
		static EnvCubeMap& getDefaultEnvCubeMap();
	};
}
