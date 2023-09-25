#include "ShaderLoader.hpp"
#include "Age/LL/Shader/ShaderModule.hpp"
#include "Age/Resource/ResourceLoader.hpp"
#include "Age/Resource/Logger.hpp"
#include "Age/Resource/File.hpp"
#include "Age/egd.hpp"

namespace a_game_engine
{
	std::unique_ptr<ShaderProgram> ShaderLoader::defShader = nullptr;
	std::unique_ptr<ShaderProgram> ShaderLoader::defPostproc = nullptr;
	std::unique_ptr<Shader> ShaderLoader::defShaderTemplate = nullptr;

	ShaderLoader::~ShaderLoader()
	{
		for (const auto& s : shaders)
		{
			if (s.second)
				Logger::logInfo("Shader " + s.first.string() + " was unloaded");
		}
	}

	Shader& ShaderLoader::loadShader(const std::filesystem::path& path)
	{
		return ResourceLoader::defaultLoad<Shader, std::filesystem::path>(shaderTemplates, path,
			[&](const std::filesystem::path& p) -> std::unique_ptr<Shader> {
				auto result = std::make_unique<Shader>(File::readAllText(p));
				return result; //TODO: nullptr if there are errors in the shader
			}, getDefaultShader);
	}

	Shader& ShaderLoader::loadShader(const std::filesystem::path& path, const ShaderSettings::Include& inc)
	{
		return ResourceLoader::defaultLoad<Shader, std::filesystem::path>(shaderTemplates, path,
			[&](const std::filesystem::path& p) -> std::unique_ptr<Shader> {
				auto result = std::make_unique<Shader>(File::readAllText(p), inc);
				return result; //TODO: nullptr if there are errors in the shader
			}, getDefaultShader);
	}

	ShaderProgram& ShaderLoader::loadRawShader(const std::filesystem::path& path)
	{
		return ResourceLoader::defaultLoad<ShaderProgram, std::filesystem::path>(shaders, path,
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
					return result;
				return nullptr;
			}, getDefault);
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
					return result;
				return nullptr;
			}, getDefault);
	}

	ShaderProgram& ShaderLoader::load(const std::filesystem::path& path)
	{
		return ResourceLoader::defaultLoad<ShaderProgram, std::filesystem::path>(shaders, path,
			[&](const std::filesystem::path& p) { return readFromFile(p); }, getDefault);
	}
	std::unique_ptr<ShaderProgram> ShaderLoader::readFromFile(const std::filesystem::path& path) const
	{
		auto result = readVertFrag(path.string() + vertexExt, path.string() + fragmentExt);
		if (result)
			Logger::logInfo("Shader " + path.string() + " was loaded");
		return result;
	}
	std::unique_ptr<ShaderProgram> ShaderLoader::readVertFrag(const std::filesystem::path& vsh,
		const std::filesystem::path& fsh)
	{
		ShaderModule vert{ File::readAllText(vsh), ShaderModule::Vertex };
		if (not vert.isValid())
			return nullptr;
		ShaderModule frag{ File::readAllText(fsh), ShaderModule::Fragment };
		if (not vert.isValid())
			return nullptr;
		std::unique_ptr<ShaderProgram> result = std::make_unique<ShaderProgram>(vert, frag);
		if (result->isValid())
			return result;

		return nullptr;
	}
	ShaderProgram& ShaderLoader::getDefault()
	{
		if (not defShader)
		{
			defShader = readVertFrag(egd.res / "shader/default.vsh", egd.res / "shader/default.fsh");
		}
		return *defShader;
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
	Shader& ShaderLoader::getDefaultShader()
	{
		if (not defShaderTemplate)
		{
			defShaderTemplate = std::make_unique<Shader>(File::readAllText(egd.res / "shader/pbrNormal.asl"));
		}
		return *defShaderTemplate;
	}
}