#include "ShaderLoader.hpp"
#include "Age/LL/Shader/ShaderCompiler.hpp"
#include "Age/Resource/ResourceLoader.hpp"
#include "Age/Resource/Logger.hpp"

namespace a_game_engine
{
	std::unique_ptr<Shader> ShaderLoader::defShader = nullptr;

	ShaderLoader::~ShaderLoader()
	{
		for (const auto& s : shaders)
		{
			if (s.second)
				Logger::logInfo("Shader " + s.first.string() + " was unloaded");
		}
	}

	Shader& ShaderLoader::load(const std::filesystem::path& path)
	{
		return ResourceLoader::defaultLoad<Shader, std::filesystem::path>(shaders, path,
			[&](const std::filesystem::path& p) { return readFromFile(p); }, getDefault);
	}
	std::unique_ptr<Shader> ShaderLoader::readFromFile(const std::filesystem::path& path) const
	{
		auto result = readVertFrag(path.string() + vertexExt, path.string() + fragmentExt);
		if (result)
			Logger::logInfo("Shader " + path.string() + " was loaded");
		return result;
	}
	std::unique_ptr<Shader> ShaderLoader::readVertFrag(const std::filesystem::path& vsh,
		const std::filesystem::path& fsh)
	{
		uint vert = ShaderCompiler::loadFromFile(vsh, ShaderCompiler::Vertex);
		if (vert == 0)
			return nullptr;
		uint frag = ShaderCompiler::loadFromFile(fsh, ShaderCompiler::Fragment);
		if (frag == 0)
			return nullptr;
		std::unique_ptr<Shader> result = std::make_unique<Shader>(vert, frag);
		if (result->isValid())
			return result;

		return nullptr;
	}
	Shader& ShaderLoader::getDefault()
	{
		if (not defShader)
		{
			defShader = readVertFrag("res/shader/light.vsh", "res/shader/light.fsh");
		}
		return *defShader;
	}
}