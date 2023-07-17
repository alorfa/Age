#pragma once

#include "Age/LL/Texture/Texture2D.hpp"
#include "Age/LL/Texture/CubeMap.hpp"
#include <map>
#include <filesystem>

namespace a_game_engine
{
	class TextureLoader
	{
		static std::unique_ptr<Texture2D> defaultTexture;
		std::map<std::filesystem::path, std::unique_ptr<Texture2D>> textures;

		static std::unique_ptr<CubeMap> defCubemap;
		std::map<std::string, std::unique_ptr<CubeMap>> cubeMaps;
	public:
		bool logOnLoad = true;
		bool logOnUnload = true;
		~TextureLoader();

		struct Settings
		{
			TextureFiltering 
				minFilter = TextureFiltering::LinearMipLinear, 
				magFilter = TextureFiltering::Linear;
			TextureWrap 
				wrapX = TextureWrap::Repeat, 
				wrapY = TextureWrap::Repeat;
			bool srgb = false;
		};

		Texture2D& load(const std::filesystem::path& path, const Settings& s = Settings());
		static std::unique_ptr<Texture2D> readFromFile(const std::filesystem::path& path, const Settings& s = Settings(),
			bool logOnLoad = true);

		CubeMap& loadCubemap();

		static Texture2D& getDefault();
	};
}