#pragma once

#include "Age/LL/Texture/Texture2D.hpp"
#include <map>
#include <filesystem>

namespace a_game_engine
{
	class TextureLoader
	{
		static std::unique_ptr<Texture2D> defaultTexture;
		std::map<std::filesystem::path, std::unique_ptr<Texture2D>> textures;
	public:
		bool logSuccessfulOperations = true;
		~TextureLoader();

		struct Settings
		{
			TextureFiltering 
				minFilter = TextureFiltering::LinearMipLinear, 
				magFilter = TextureFiltering::Linear;
			TextureWrap 
				wrapX = TextureWrap::Repeat, 
				wrapY = TextureWrap::Repeat;
		};

		Texture2D& load(const std::filesystem::path& path, const Settings& s = Settings());
		static std::unique_ptr<Texture2D> readFromFile(const std::filesystem::path& path, const Settings& s = Settings(),
			bool logSuccess = true);
		static Texture2D& getDefault();
	};
}