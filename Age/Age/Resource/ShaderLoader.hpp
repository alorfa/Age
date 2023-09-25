#pragma once

#include "Age/LL/Shader/ShaderProgram.hpp"
#include <filesystem>
#include <map>
#include "Age/Material/Shader.hpp"

namespace a_game_engine
{
	class ShaderLoader
	{
		static std::unique_ptr<ShaderProgram> defShader, defPostproc;
		static std::unique_ptr<Shader> defShaderTemplate;
		std::map<std::filesystem::path, std::unique_ptr<ShaderProgram>> shaders;
		std::map<std::filesystem::path, std::unique_ptr<ShaderProgram>> postp;
		std::map<std::filesystem::path, std::unique_ptr<Shader>> shaderTemplates;
	public:
		std::string vertexExt = ".vsh", fragmentExt = ".fsh";

		~ShaderLoader();

		Shader& loadShader(const std::filesystem::path& path);
		Shader& loadShader(const std::filesystem::path& path, const ShaderSettings::Include& inc);
		ShaderProgram& loadRawShader(const std::filesystem::path& path);
		ShaderProgram& loadPostproc(const std::filesystem::path& path);

		ShaderProgram& load(const std::filesystem::path& path);
		std::unique_ptr<ShaderProgram> readFromFile(const std::filesystem::path& path) const;
		static std::unique_ptr<ShaderProgram> readVertFrag(const std::filesystem::path& vsh,
			const std::filesystem::path& fsh);
		static Shader& getDefaultShader();
		static ShaderProgram& getDefault();
		static ShaderProgram& getDefaultPostproc();
	};
}
