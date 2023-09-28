#pragma once

#include "Age/LL/Shader/ShaderProgram.hpp"
#include <filesystem>
#include <map>
#include "Age/Material/Shader.hpp"

namespace a_game_engine
{
	class ShaderLoader
	{
		static std::unique_ptr<ShaderProgram> defRawShader, defPostproc;
		static std::unique_ptr<Shader> defShader;
		std::map<std::filesystem::path, std::unique_ptr<ShaderProgram>> rawShaders;
		std::map<std::filesystem::path, std::unique_ptr<ShaderProgram>> postp;
		std::map<std::filesystem::path, std::unique_ptr<Shader>> shaders;
	public:
		~ShaderLoader();

		Shader& load(const std::filesystem::path& path);
		Shader& load(const std::filesystem::path& path, const ShaderSettings::Include& inc);
		ShaderProgram& loadRaw(const std::filesystem::path& path);
		ShaderProgram& loadPostproc(const std::filesystem::path& path);

		static Shader& getDefault();
		static ShaderProgram& getDefaultRaw();
		static ShaderProgram& getDefaultPostproc();
	};
}
