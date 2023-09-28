#include "ShaderLoader.hpp"
#include "Age/LL/Shader/ShaderModule.hpp"
#include "Age/Resource/ResourceLoader.hpp"
#include "Age/Resource/Logger.hpp"
#include "Age/Resource/File.hpp"
#include "Age/egd.hpp"

namespace a_game_engine
{
	std::unique_ptr<ShaderProgram> ShaderLoader::defRawShader = nullptr;
	std::unique_ptr<ShaderProgram> ShaderLoader::defPostproc = nullptr;
	std::unique_ptr<Shader> ShaderLoader::defShader = nullptr;

	ShaderLoader::~ShaderLoader()
	{
		for (const auto& s : rawShaders)
		{
			if (s.second)
				Logger::logInfo("Raw shader " + s.first.string() + " was unloaded");
		}
		for (const auto& s : postp)
		{
			if (s.second)
				Logger::logInfo("Postprocessing " + s.first.string() + " was unloaded");
		}
		for (const auto& s : shaders)
		{
			if (s.second)
				Logger::logInfo("Shader " + s.first.string() + " was unloaded");
		}
	}

	Shader& ShaderLoader::load(const std::filesystem::path& path)
	{
		return ResourceLoader::defaultLoad<Shader, std::filesystem::path>(shaders, path,
			[&](const std::filesystem::path& p) -> std::unique_ptr<Shader> {
				auto result = std::make_unique<Shader>(File::readAllText(p));
				Logger::logInfo("Shader " + p.string() + " was loaded");
				return result; //TODO: nullptr if there are errors in the shader
			}, getDefault);
	}

	Shader& ShaderLoader::load(const std::filesystem::path& path, const ShaderSettings::Include& inc)
	{
		return ResourceLoader::defaultLoad<Shader, std::filesystem::path>(shaders, path,
			[&](const std::filesystem::path& p) -> std::unique_ptr<Shader> {
				auto result = std::make_unique<Shader>(File::readAllText(p), inc);
				Logger::logInfo("Shader " + p.string() + " was loaded");
				return result; //TODO: nullptr if there are errors in the shader
			}, getDefault);
	}

	ShaderProgram& ShaderLoader::loadRaw(const std::filesystem::path& path)
	{
		return ResourceLoader::defaultLoad<ShaderProgram, std::filesystem::path>(rawShaders, path,
			[&](const std::filesystem::path& p) -> std::unique_ptr<ShaderProgram> {

				std::string source = File::readAllText(p);
				ShaderSettings::Detailed settings{ShaderSettings::rawInclude};
				auto [vsh, fsh] = Shader::translateToGlsl(settings, source);

				ShaderModule vert{ vsh, ShaderModule::Vertex };
				if (not vert.isValid())
					return nullptr;
				ShaderModule frag{ fsh, ShaderModule::Fragment };
				if (not frag.isValid())
					return nullptr;

				auto result = std::make_unique<ShaderProgram>(vert, frag);
				if (result->isValid())
				{
					Logger::logInfo("Raw shader " + p.string() + " was loaded");
					return result;
				}
				return nullptr;
			}, getDefaultRaw);
	}

	ShaderProgram& ShaderLoader::loadPostproc(const std::filesystem::path& path)
	{
		return ResourceLoader::defaultLoad<ShaderProgram, std::filesystem::path>(postp, path,
			[&](const std::filesystem::path& p) -> std::unique_ptr<ShaderProgram> {

				std::string source = File::readAllText(p);

				ShaderModule vert{ ShaderSettings::postprocVsh, ShaderModule::Vertex };
				if (not vert.isValid())
					return nullptr;
				ShaderModule frag{ source, ShaderModule::Fragment };
				if (not frag.isValid())
					return nullptr;

				auto result = std::make_unique<ShaderProgram>(vert, frag);
				if (result->isValid())
				{
					Logger::logInfo("Postprocessing " + p.string() + " was loaded");
					return result;
				}
				return nullptr;
			}, getDefaultRaw);
	}

	ShaderProgram& ShaderLoader::getDefaultRaw()
	{
		if (not defRawShader)
		{
			std::string source = File::readAllText(egd.res / "shader/skybox.rasl");

			ShaderSettings::Detailed settings{ShaderSettings::rawInclude};
			auto [vsh, fsh] = Shader::translateToGlsl(settings, source);
			ShaderModule vert{ vsh, ShaderModule::Vertex };
			ShaderModule frag{ fsh, ShaderModule::Fragment };

			defRawShader = std::make_unique<ShaderProgram>(vert, frag);
		}
		return *defRawShader;
	}
	ShaderProgram& ShaderLoader::getDefaultPostproc()
	{
		if (not defPostproc)
		{
			std::string source = File::readAllText(egd.res / "shader/defpostp.pasl");

			ShaderModule vert{ ShaderSettings::postprocVsh, ShaderModule::Vertex };
			ShaderModule frag{ source, ShaderModule::Fragment };

			defPostproc = std::make_unique<ShaderProgram>(vert, frag);
		}
		return *defPostproc;
	}
	Shader& ShaderLoader::getDefault()
	{
		if (not defShader)
		{
			defShader = std::make_unique<Shader>(File::readAllText(egd.res / "shader/pbrNormal.asl"));
		}
		return *defShader;
	}
}