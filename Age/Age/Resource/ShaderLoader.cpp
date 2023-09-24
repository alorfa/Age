#include "ShaderLoader.hpp"
#include "Age/LL/Shader/ShaderModule.hpp"
#include "Age/Resource/ResourceLoader.hpp"
#include "Age/Resource/Logger.hpp"
#include "Age/Resource/File.hpp"

namespace a_game_engine
{
	std::unique_ptr<ShaderProgram> ShaderLoader::defShader = nullptr;

	ShaderLoader::~ShaderLoader()
	{
		for (const auto& s : shaders)
		{
			if (s.second)
				Logger::logInfo("Shader " + s.first.string() + " was unloaded");
		}
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
			defShader = readVertFrag("res/shader/default.vsh", "res/shader/default.fsh");
		}
		return *defShader;
	}
}