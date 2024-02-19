#pragma once

#include <map>
#include "Age/LL/Texture/Texture2D.hpp"
#include "Age/LL/Texture/CubeMap.hpp"
#include "Image.hpp"

namespace a_game_engine
{
	class TextureLoader
	{
		static std::unique_ptr<Image> defaultImage;

		static std::unique_ptr<Texture2D> defaultTexture;
		std::map<std::filesystem::path, std::unique_ptr<Texture2D>> textures;
	public:
		static std::filesystem::path defaultImagePath;

		bool logOnLoad = true;
		bool logOnUnload = true;
		~TextureLoader();

		struct Settings
		{
			Sampler2DInfo sampler;
			TextureFormat format;
			MipmapSettings mipmaps;

			Settings(
				const Sampler2DInfo& sampler = Sampler2DInfo{},
				TextureFormat format = TextureFormat::AutoSRGB,
				MipmapSettings mipmaps = MipmapSettings::Auto);

			bool hasMipmaps() const;
		};

		Texture2D& load(const std::filesystem::path& path, const Settings& s = {});

		static Texture2D& getDefault();
		static Image& getDefaultImage();
	};
}