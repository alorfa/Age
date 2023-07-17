#include "TextureLoader.hpp"
#include "Age/LL/Texture/Image.hpp"
#include "Age/Resource/ResourceLoader.hpp"
#include "Age/Resource/Logger.hpp"

namespace a_game_engine
{
	std::unique_ptr<Texture2D> TextureLoader::defaultTexture = nullptr;

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
			[&](const std::filesystem::path& p) { return readFromFile(path, s, logOnLoad); },
			getDefault);
	}
	std::unique_ptr<Texture2D> TextureLoader::readFromFile(const std::filesystem::path& path, const Settings& s,
		bool logSuccess)
	{
		Image img;
		img.loadFromFile(path);
		if (img.getInfo().size.x == 0 or img.getInfo().size.y == 0)
			return nullptr;

		bool mipmaps = s.minFilter >= TextureFiltering::LinearMipLinear && s.minFilter <= TextureFiltering::NearMipNear;
		auto result = std::make_unique<Texture2D>();
		Texture2D::Settings settings = { img.getInfo(), mipmaps };
		if (s.srgb)
		{
			if (img.getInfo().format == TextureFormat::RGB)
				settings.internal = TextureFormat::SRGB;
			else
				settings.internal = TextureFormat::SRGB_Alpha;
		}
		result->create(settings);
		result->setWrap(s.wrapX, s.wrapY);
		result->setFiltering(s.minFilter, s.magFilter);
		if (logSuccess)
			Logger::logInfo("Image " + path.string() + " was loaded");
		return result;
	}
	Texture2D& TextureLoader::getDefault()
	{
		if (defaultTexture == nullptr)
			defaultTexture = readFromFile("res/img/default.jpg");
		return *defaultTexture;
	}
}