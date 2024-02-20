#include "CubeMapLoader.hpp"
#include "TextureLoader.hpp"
#include "ResourceLoader.hpp"

namespace a_game_engine
{
	std::unique_ptr<CubeMap> CubeMapLoader::defCubemap = nullptr;
	std::unique_ptr<EnvCubeMap> CubeMapLoader::defEnvCubemap = nullptr;

	std::unique_ptr<CubeMap> CubeMapLoader::readFromFile(const std::filesystem::path& path, const RawSettings& s)
	{
		Image img;
		img.loadFromFile(path);
		if (not img.info.isValid())
			return nullptr;

		Sampler2DInfo sampler = { TextureFiltering::Linear, TextureWrap::Repeat };
		Texture2D::Settings settings = { img.info, TextureFormat::Auto, sampler, 1 };
		Texture2D panorama(settings);

		auto result = std::make_unique<CubeMap>();
		CubeMap::PanoramaSettings ps{ panorama, s.size, s.format, s.sampler, s.hasMipmaps() ? -1 : 1, s.srgb };
		result->createFromPanorama(ps);
		return result;
	}

	const CubeMap& CubeMapLoader::loadRaw(const std::filesystem::path& path, const RawSettings& s)
	{
		return ResourceLoader::defaultLoad<CubeMap, std::filesystem::path>(rawCubeMaps, path,
			[&](const std::filesystem::path& p)
			{
				return readFromFile(path, s);
			},
			getDefaultCubeMap);
	}

	const EnvCubeMap& CubeMapLoader::load(const std::filesystem::path& path, const Settings& s)
	{
		return ResourceLoader::defaultLoad<EnvCubeMap, std::filesystem::path>(cubeMaps, path,
			[&](const std::filesystem::path& p) -> std::unique_ptr<EnvCubeMap>
			{
				RawSettings settings = { {TextureFiltering::Linear}, s.tempFormat, s.specularSize, 
					MipmapSettings::Auto, s.srgb };
				auto cubemap = readFromFile(path, settings);
				if (cubemap == nullptr)
					return nullptr;

				auto result = std::make_unique<EnvCubeMap>();
				result->specular.createSpecularMap(*cubemap, s.specularFormat);
				if (s.diffuseSize > 0)
					result->diffuse.createDiffuseMap(*cubemap, s.diffuseSize, s.diffuseFormat);
				return result;
			},
			getDefaultEnvCubeMap);
	}

	CubeMap& CubeMapLoader::getDefaultCubeMap()
	{
		if (defCubemap == nullptr)
		{
			const auto& img = TextureLoader::getDefaultImage();
			ImageInfo info[6];
			for (uint i = 0; i < 6; i++)
			{
				info[i].data = img.info.data;
				info[i].format = img.info.format;
			}
			CubeMap::Settings settings{ info, img.info.size.y, TextureFormat::SRGB,
				SamplerCubeInfo{TextureFiltering::Near}, 1 };
			defCubemap = std::make_unique<CubeMap>();
			defCubemap->create(settings);
		}
		return *defCubemap;
	}

	EnvCubeMap& CubeMapLoader::getDefaultEnvCubeMap()
	{
		if (defEnvCubemap == nullptr)
		{
			const auto& img = TextureLoader::getDefaultImage();
			ImageInfo info[6];
			for (uint i = 0; i < 6; i++)
			{
				info[i].data = img.info.data;
				info[i].format = img.info.format;
			}
			CubeMap::Settings settings{ info, img.info.size.y, TextureFormat::SRGB,
				SamplerCubeInfo{TextureFiltering::Near}, 1 };
			defEnvCubemap = std::make_unique<EnvCubeMap>();
			defEnvCubemap->specular.create(settings);
		}
		return *defEnvCubemap;
	}

	CubeMapLoader::Settings::Settings(TextureFormat specularFormat,
		TextureFormat diffuseFormat, TextureFormat tempFormat, bool srgb, uint specularSize, uint diffuseSize)
		: specularFormat(specularFormat), diffuseFormat(diffuseFormat), tempFormat(tempFormat),
		srgb(srgb), specularSize(specularSize), diffuseSize(diffuseSize)
	{}
	bool CubeMapLoader::RawSettings::hasMipmaps() const
	{
		if (mipmaps == MipmapSettings::Disable)
			return false;

		if (mipmaps == MipmapSettings::Enable)
			return true;

		return sampler.min >= TextureFiltering::Linear_MipLinear && sampler.min <= TextureFiltering::Near_MipNear;
	}
	/*CubeMapLoader::EnvCubeMapSettings::EnvCubeMapSettings(TextureFormat format, bool srgb,
		uint specularSize, uint diffuseSize)
	{

	}*/
}
