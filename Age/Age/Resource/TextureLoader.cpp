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
					
				bool mipmaps = s.minFilter >= TextureFiltering::LinearMipLinear && 
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