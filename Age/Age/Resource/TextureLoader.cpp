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
	std::unique_ptr<Texture2D> TextureLoader::brdfLut = nullptr;
	std::unique_ptr<Image> TextureLoader::defaultImage = nullptr;
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
	Texture2D& TextureLoader::getBrdfLut()
	{
		if (brdfLut == nullptr)
		{
			brdfLut = std::make_unique<Texture2D>();
			ImageInfo img;
			img.size = uvec2(512u);
			Sampler2DInfo sampler{ TextureFiltering::Linear };
			Texture2D::Settings s{ img, TextureFormat::RGB_F16, sampler, 1 }; //TODO: add RG16
			brdfLut->create(s);

			auto& shader = egd.shaders.loadPostproc(egd.res / "shader/GenBrdfLut.pasl");
			const auto& buf = VertexBuffer::getDefFramebuf();
			FrameBuffer2D fb;
			fb.setTexture(0, *brdfLut);
			fb.use();
			shader.use();
			buf.draw();
		}
		return *brdfLut;
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
}