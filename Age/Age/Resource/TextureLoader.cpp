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

	CubeMap& TextureLoader::loadCubeMap(const std::filesystem::path& path, const CubeMapSettings& s)
	{
		return ResourceLoader::defaultLoad<CubeMap, std::filesystem::path>(newCubeMaps, path,
			[&](const std::filesystem::path& p) -> std::unique_ptr<CubeMap>
			{
				Image img;
				img.loadFromFile(path);
				if (not img.info.isValid())
					return std::unique_ptr<CubeMap>(nullptr);

				Sampler2DInfo sampler = { TextureWrap::Repeat, TextureFiltering::Linear };
				Texture2D::Settings settings = { img.info, TextureFormat::Auto, sampler, 1 };
				Texture2D sphereMap(settings);

				const vec3 x = { 1.f, 0.f, 0.f };
				const vec3 y = { 0.f, 1.f, 0.f };
				const vec3 z = { 0.f, 0.f, 1.f };
				mat4 proj;
				proj.setPerspective(Math::rad(90.f), 1.f, 0.1f, 10.f);
				mat4 view[6];
				view[0].setViewMatrix({ 0.f }, -z, -y, -x);
				view[1].setViewMatrix({ 0.f }, z, -y, x);
				view[2].setViewMatrix({ 0.f }, x, -z, y);
				view[3].setViewMatrix({ 0.f }, x, z, -y);
				view[4].setViewMatrix({ 0.f }, x, -y, -z);
				view[5].setViewMatrix({ 0.f }, -x, -y, z);

				auto& shader = egd.shaders.loadRaw(egd.res / "shader/tex2cubemap.rasl");

				Pipeline::set2DContext();

				const bool mipmaps = s.hasMipmaps();
				const uint faceSize = s.resolution < 1 ? img.info.size.y / 2 : s.resolution;
				const TextureFormat format = TexEnums::chooseInternalFormat(img.info.format, s.internalFormat);
				CubeMap::Settings cubeSettings{ nullptr, faceSize, format, s.sampler, mipmaps ? -1 : 1 };
				auto result = std::make_unique<CubeMap>();
				result->create(cubeSettings);
				
				FrameBuffer2D fb;

				for (uint i = 0; i < 6; i++)
				{
					fb.setCubemapTexture(0, *result, CubeMap::Face(i));
					fb.use();
					shader.use();
					shader.setUniform(shader.getLocation("sphereMap"), sphereMap, 0);
					shader.setUniform(shader.getLocation("projection"), proj);
					shader.setUniform(shader.getLocation("view"), view[i]);
					const auto& skyboxMesh = egd.models.load(egd.res / "model/skybox.obj");
					skyboxMesh.meshes[0]->buffer.draw();
				}

				return result;
			}, 
			getDefaultCubeMap);
	}
	void TextureLoader::removeCubeMap(const std::filesystem::path& path)
	{
		newCubeMaps.erase(path);
	}
	/*EnvCubeMap& TextureLoader::loadEnv(const std::filesystem::path& path, const CubeMapSettings& s)
	{

	}*/
	Texture2D& TextureLoader::getDefault()
	{
		if (defaultTexture == nullptr)
		{
			const auto& img = getDefaultImage();
			defaultTexture = std::make_unique<Texture2D>();
			defaultTexture->create({ 
				img.info, 
				TextureFormat::Auto, 
				Sampler2DInfo{TextureWrap::Repeat, TextureFiltering::Linear}, 
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
				SamplerCubeInfo{TextureWrap::Repeat, TextureFiltering::Near}, 1 };
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