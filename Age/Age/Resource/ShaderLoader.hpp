#pragma once

#include "Age/LL/Shader/Shader.hpp"
#include <filesystem>
#include <map>

namespace a_game_engine
{
	class ShaderLoader
	{
		static std::unique_ptr<Shader> defShader;
		std::map<std::filesystem::path, std::unique_ptr<Shader>> shaders;
	public:
		std::string vertexExt = ".vsh", fragmentExt = ".fsh";

		~ShaderLoader();

		Shader& load(const std::filesystem::path& path);
		std::unique_ptr<Shader> readFromFile(const std::filesystem::path& path) const;
		static std::unique_ptr<Shader> readVertFrag(const std::filesystem::path& vsh,
			const std::filesystem::path& fsh);
		static Shader& getDefault();
	};
}
