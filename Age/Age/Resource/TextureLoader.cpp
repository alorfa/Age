#include "TextureLoader.hpp"
#include "Age/egd.hpp"
#include "Age/Resource/ResourceLoader.hpp"
#include "Age/Other/Logger.hpp"
#include "Age/egd.hpp"
#include "Age/Math/Math.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/Object/Mesh.hpp"

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
				result->setFiltering(s.minFilter, s.magFilter); //TODO: log
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
	CubeMap& TextureLoader::loadCubeMap(const std::filesystem::path& path, const CubemapSettings& s)
	{
		return ResourceLoader::defaultLoad<CubeMap, std::filesystem::path>(newCubeMaps, path,
			[&](const std::filesystem::path& p) -> std::unique_ptr<CubeMap>
			{
				Image img;
				img.loadFromFile(path);
				if (not img.info.isValid())
					return std::unique_ptr<CubeMap>(nullptr);

				Texture2D::Settings settings = { img.info, false };
				Texture2D sphereMap(settings);

				const float deg90 = Math::rad(90.f);
				mat4 proj;
				proj.setPerspective(deg90, 1.f, 0.1f, 10.f);
				mat4 view[6];
				view[0].setViewMatrix({ 0.f }, { 0.f, -deg90, 0.f });
				view[1].setViewMatrix({ 0.f }, { 0.f, deg90, 0.f });
				view[2].setViewMatrix({ 0.f }, {0.f});
				view[3].setViewMatrix({ 0.f }, {0.f, Math::PI, 0.f});
				view[4].setViewMatrix({ 0.f }, {});
				view[5].setViewMatrix({ 0.f }, {});

				auto& shader = egd.shaders.loadRaw(egd.res / "shader/tex2cubemap.rasl");

				const bool mipmaps = s.minFilter >= TextureFiltering::LinearMipLinear &&
					s.minFilter <= TextureFiltering::NearMipNear;
				const uint faceSize = s.faceSize < 0 ? img.info.size.y / 2 : s.faceSize;
				ImageInfo images[6] = {
					{uvec2{faceSize}, nullptr, s.internalFormat},
					{uvec2{faceSize}, nullptr, s.internalFormat},
					{uvec2{faceSize}, nullptr, s.internalFormat},
					{uvec2{faceSize}, nullptr, s.internalFormat},
					{uvec2{faceSize}, nullptr, s.internalFormat},
					{uvec2{faceSize}, nullptr, s.internalFormat}
				};
				CubeMap::Settings cubeSettings{ images, faceSize, s.internalFormat, mipmaps };
				auto result = std::make_unique<CubeMap>();
				result->create(cubeSettings);
				result->setWrap(s.wrapX, s.wrapY, s.wrapZ);
				result->setFiltering(s.minFilter, s.magFilter);
				
				FrameBuffer2D fb;

				for (uint i = 0; i < 6; i++)
				{
					fb.setCubemapTexture(0, *result, CubeMap::Face(i));
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