#include "TextureLoader.hpp"
#include "Age/egd.hpp"
#include "Age/Resource/ResourceLoader.hpp"
#include "Age/Other/Logger.hpp"
#include "Age/egd.hpp"
#include "Age/Math/Math.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/LL/Pipeline.hpp"

namespace a_game_engine
{
	std::unique_ptr<Texture2D> TextureLoader::defaultTexture = nullptr;
	std::unique_ptr<Image> TextureLoader::defaultImage = nullptr;
	std::unique_ptr<CubeMap> TextureLoader::defCubemap = nullptr;
	std::unique_ptr<EnvCubeMap> TextureLoader::defEnvCubemap = nullptr;
	std::filesystem::path TextureLoader::defaultImagePath = "res/img/default.jpg";

	TextureLoader::~TextureLoader()
	{
		if (logOnUnload)
			for (const auto& t : textures)
			{
				if (t.second)
					Logger::logInfo("Image " + t.first.string() + " was unloaded");
			}
	}

	Texture2D& TextureLoader::load(const std::filesystem::path& path, const Settings& s)
	{
		return ResourceLoader::defaultLoad<Texture2D, std::filesystem::path>(textures, path, 
			[&](const std::filesystem::path& p) 
			{ 
				Image img;
				img.loadFromFile(path);
				if (not img.info.isValid())
					return std::unique_ptr<Texture2D>(nullptr);
					
				const bool mipmaps = s.hasMipmaps();
				auto result = std::make_unique<Texture2D>();
				Texture2D::Settings settings{ img.info, s.format, s.sampler, mipmaps ? -1 : 1 };
				result->create(settings);
				return result;
			},
			getDefault);
	}
	std::unique_ptr<CubeMap> TextureLoader::readCubeMapFromFile(const std::filesystem::path& path, const CubeMapSettings& s)
	{
		Image img;
		img.loadFromFile(path);
		if (not img.info.isValid())
			return std::unique_ptr<CubeMap>(nullptr);

		Sampler2DInfo sampler = { TextureFiltering::Linear, TextureWrap::Repeat };
		Texture2D::Settings settings = { img.info, TextureFormat::Auto, sampler, 1 };
		Texture2D panorama(settings);

		auto result = std::make_unique<CubeMap>();
		CubeMap::PanoramaSettings ps{ panorama, s.size, s.format, s.sampler, s.hasMipmaps() ? -1 : 1 };
		result->createFromPanorama(ps);
		return result;
	}

	CubeMap& TextureLoader::loadCubeMap(const std::filesystem::path& path, const CubeMapSettings& s)
	{
		return ResourceLoader::defaultLoad<CubeMap, std::filesystem::path>(newCubeMaps, path,
			[&](const std::filesystem::path& p)
			{
				return readCubeMapFromFile(path, s);
			}, 
			getDefaultCubeMap);
	}
	EnvCubeMap& TextureLoader::loadEnv(const std::filesystem::path& path, const CubeMapSettings& s)
	{
		return ResourceLoader::defaultLoad<EnvCubeMap, std::filesystem::path>(cubeMaps, path,
			[&](const std::filesystem::path& p) -> std::unique_ptr<EnvCubeMap>
			{
				auto cubemap = readCubeMapFromFile(path, s);
				if (cubemap == nullptr)
					return nullptr;

				auto result = std::make_unique<EnvCubeMap>();
				return nullptr;
			},
			getDefaultEnvCubeMap);
	}
	EnvCubeMap& TextureLoader::getDefaultEnvCubeMap()
	{
		if (defEnvCubemap == nullptr)
		{
			const auto& img = getDefaultImage();
			ImageInfo info[6];
			for (uint i = 0; i < 6; i++)
			{
				info[i].data = img.info.data;
				info[i].format = img.info.format;
			}
			CubeMap::Settings settings{ info, img.info.size.y, TextureFormat::AutoSRGB,
				SamplerCubeInfo{TextureFiltering::Near}, 1 };
			defEnvCubemap = std::make_unique<EnvCubeMap>();
			defEnvCubemap->specular.create(settings);
		}
		return *defEnvCubemap;
	}
	void TextureLoader::removeCubeMap(const std::filesystem::path& path)
	{
		newCubeMaps.erase(path);
	}
	Texture2D& TextureLoader::getDefault()
	{
		if (defaultTexture == nullptr)
		{
			const auto& img = getDefaultImage();
			defaultTexture = std::make_unique<Texture2D>();
			defaultTexture->create({ 
				img.info, 
				TextureFormat::Auto, 
				Sampler2DInfo{ TextureFiltering::Linear, TextureWrap::Repeat },
				1 });
		}
		return *defaultTexture;
	}
	CubeMap& TextureLoader::getDefaultCubeMap()
	{
		if (defCubemap == nullptr)
		{
			const auto& img = getDefaultImage();
			ImageInfo info[6];
			for (uint i = 0; i < 6; i++)
			{
				info[i].data = img.info.data;
				info[i].format = img.info.format;
			}
			CubeMap::Settings settings{ info, img.info.size.y, TextureFormat::AutoSRGB,
				SamplerCubeInfo{TextureFiltering::Near}, 1 };
			defCubemap = std::make_unique<CubeMap>();
			defCubemap->create(settings);
		}
		return *defCubemap;
	}
	Image& TextureLoader::getDefaultImage()
	{
		if (defaultImage == nullptr)
		{
			defaultImage = std::make_unique<Image>();
			defaultImage->loadFromFile(defaultImagePath);
			if (not defaultImage->info.isValid())
			{
				defaultImage->info.data = new ubyte[]
				{
					0x00, 0xFF, 0x00, 0xFF,
					0xFF, 0x00, 0xFF, 0x00,
					0x00, 0xFF, 0x00, 0xFF,
					0xFF, 0x00, 0xFF, 0x00
				};
				defaultImage->info.format = TextureFormat::R;
				defaultImage->info.size = { 4, 4 };
			}
		}
		return *defaultImage;
	}
	TextureLoader::Settings::Settings(const Sampler2DInfo& sampler, TextureFormat format, MipmapSettings mipmaps)
		: sampler(sampler), format(format), mipmaps(mipmaps) {}

	bool TextureLoader::Settings::hasMipmaps() const
	{
		if (mipmaps == MipmapSettings::Disable)
			return false;
		if (mipmaps == MipmapSettings::Enable)
			return true;

		return sampler.min >= TextureFiltering::Linear_MipLinear && sampler.min <= TextureFiltering::Near_MipNear;
	}
	bool TextureLoader::CubeMapSettings::hasMipmaps() const
	{
		if (mipmaps == MipmapSettings::Disable)
			return false;
		return true;
	}
}