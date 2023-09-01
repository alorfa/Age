#include "TextureLoader.hpp"
#include "Age/LL/Texture/Image.hpp"
#include "Age/Resource/ResourceLoader.hpp"
#include "Age/Resource/Logger.hpp"

namespace a_game_engine
{
	std::unique_ptr<Texture2D> TextureLoader::defaultTexture = nullptr;
	std::unique_ptr<Image> TextureLoader::defaultImage = nullptr;
	std::unique_ptr<CubeMap> TextureLoader::defCubemap = nullptr;
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
					
				const bool mipmaps = s.minFilter >= TextureFiltering::LinearMipLinear && 
					s.minFilter <= TextureFiltering::NearMipNear;
				auto result = std::make_unique<Texture2D>();
				Texture2D::Settings settings = { img.info, mipmaps };
				if (s.srgb)
				{
					if (img.info.format == TextureFormat::RGB)
						settings.internal = TextureFormat::SRGB;
					else
						settings.internal = TextureFormat::SRGB_Alpha;
				}
				result->create(settings);
				result->setWrap(s.wrapX, s.wrapY);
				result->setFiltering(s.minFilter, s.magFilter);
				return result;
			},
			getDefault);
	}
	CubeMap& TextureLoader::loadCubeMap(const std::filesystem::path* paths, const CubemapSettings& s)
	{
		std::wstring strPaths;
		for (uint i = 0; i < 6; i++)
			strPaths += std::format(L"{}\n", paths[i].native());

		return ResourceLoader::defaultLoad<CubeMap, std::wstring>(cubeMaps, strPaths,
			[&](const std::wstring& strPaths)
			{
				Image img[6];
				ImageInfo images[6];
				for (uint i = 0; i < 6; i++)
				{
					img[i].loadFromFile(paths[i]);
					if (not img[i].info.isValid())
						return std::unique_ptr<CubeMap>(nullptr);
					images[i].data = img[i].info.data;
					images[i].format = img[i].info.format;
				}

				const bool mipmaps = s.minFilter >= TextureFiltering::LinearMipLinear &&
					s.minFilter <= TextureFiltering::NearMipNear;
				const uint imageSize = s.faceSize == -1 ? img[0].info.size.y : s.faceSize;
				CubeMap::Settings cubeSettings{images, imageSize, s.internalFormat, mipmaps};
				auto result = std::make_unique<CubeMap>();
				result->create(cubeSettings);
				result->setWrap(s.wrapX, s.wrapY, s.wrapZ);
				result->setFiltering(s.minFilter, s.magFilter);
				return result;
			},
			getDefaultCubeMap);
	}
	Texture2D& TextureLoader::getDefault()
	{
		if (defaultTexture == nullptr)
		{
			const auto& img = getDefaultImage();
			Texture2D::Settings settings = { img.info, false };
			defaultTexture = std::make_unique<Texture2D>(settings);
			defaultTexture->setWrap(TextureWrap::Repeat);
			defaultTexture->setFiltering(TextureFiltering::Near);
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
			CubeMap::Settings s = { info, img.info.size.y, TextureFormat::SRGB, false };
			defCubemap = std::make_unique<CubeMap>();
			defCubemap->create(s);
			defCubemap->setWrap(TextureWrap::ClampToEdge);
			defCubemap->setFiltering(TextureFiltering::Near);
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
}