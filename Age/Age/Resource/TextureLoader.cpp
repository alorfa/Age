#include "TextureLoader.hpp"
#include "Age/LL/Texture/Image.hpp"
#include "Age/Resource/ResourceLoader.hpp"
#include "Age/Resource/Logger.hpp"

namespace a_game_engine
{
	std::unique_ptr<Texture2D> TextureLoader::defaultTexture = nullptr;

	TextureLoader::~TextureLoader()
	{
		if (logSuccessfulOperations)
			for (const auto& t : textures)
			{
				if (t.second)
					Logger::logInfo("Texture " + t.first.string() + " was unloaded");
			}
	}

	Texture2D& TextureLoader::load(const std::filesystem::path& path, const Settings& s)
	{
		return ResourceLoader::defaultLoad<Texture2D, std::filesystem::path>(textures, path, 
			[&](const std::filesystem::path& p) { return readFromFile(path, s, logSuccessfulOperations); },
			getDefault);
	}
	std::unique_ptr<Texture2D> TextureLoader::readFromFile(const std::filesystem::path& path, const Settings& s,
		bool logSuccess)
	{
		Image img;
		img.loadFromFile(path);
		if (img.getSize().x == 0 or img.getSize().y == 0)
			return nullptr;

		bool mipmaps = s.minFilter >= TextureFiltering::LinearMipLinear && s.minFilter <= TextureFiltering::NearMipNear;
		auto result = std::make_unique<Texture2D>();
		result->create(Texture2D::Settings{img.getSize(), img.getData(), img.getFormat(), 
			TextureDataType::Ubyte, mipmaps});
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