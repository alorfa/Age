#pragma once

#include "Age/LL/Shader/ShaderProgram.hpp"
#include <filesystem>
#include <map>

namespace a_game_engine
{
	class ShaderLoader
	{
		static std::unique_ptr<ShaderProgram> defShader;
		std::map<std::filesystem::path, std::unique_ptr<ShaderProgram>> shaders;
	public:
		std::string vertexExt = ".vsh", fragmentExt = ".fsh";

		~ShaderLoader();

		ShaderProgram& load(const std::filesystem::path& path);
		std::unique_ptr<ShaderProgram> readFromFile(const std::filesystem::path& path) const;
		static std::unique_ptr<ShaderProgram> readVertFrag(const std::filesystem::path& vsh,
			const std::filesystem::path& fsh);
		static ShaderProgram& getDefault();
	};
}
