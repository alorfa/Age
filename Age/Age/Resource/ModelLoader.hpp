#pragma once

#include "Age/Object/Model.hpp"
#include "TextureLoader.hpp"

namespace a_game_engine
{
	class ModelLoader
	{
		static std::unique_ptr<Model> defModel;
		std::map<std::filesystem::path, std::unique_ptr<Model>> resources;
		TextureLoader textureLoader;
	public:
		ModelLoader();
		~ModelLoader();

		struct Settings
		{
			mat4 rootNodeTransform;
			bool inverseUV = false;
			bool withNormalMap = false;
			bool disableSrgb = false;

			Settings(const mat4 & mat, bool inverseUv = false, bool withNormalMap = false,
				bool disableSrgb = false);
			Settings(const vec3& scale, bool rotate, bool inverseUv = false, bool withNormalMap = false, 
				bool disableSrgb = false);
			Settings() = default;
		};

		Model& load(const std::filesystem::path& path, const Settings& s = Settings{});
		std::unique_ptr<Model> readFromFile(const std::filesystem::path& path, 
			const Settings& s = Settings{});
		static Model& getDefault();
	};
}
