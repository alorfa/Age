#include "File.hpp"
#include <fstream>

namespace a_game_engine
{
	std::vector<ubyte> File::readAllBytes(const std::filesystem::path& path)
	{
		std::vector<ubyte> result;

		std::ifstream stream(path, std::ios_base::in | std::ios_base::ate | std::ios_base::binary);
		if (not stream.is_open())
		{
			Logger::logError("Binary file " + path.string() + " not found");
			return result;
		}

		size_t filesize = (size_t)stream.tellg();
		if (not filesize)
		{
			Logger::logError("Binary file " + path.string() + " is void");
			return result;
		}
		stream.seekg(0, std::ios_base::beg);
		result.resize(filesize);
		stream.read((char*)result.data(), filesize);

		return result;
	}

	std::string File::readAllText(const std::filesystem::path& path) 
	{
		std::string result;

		std::ifstream stream(path, std::ios_base::in | std::ios_base::ate);
		if (not stream.is_open())
		{
			Logger::logError("Text file " + path.string() + " not found");
			return result;
		}

		size_t filesize = (size_t)stream.tellg();
		if (not filesize)
		{
			Logger::logError("Text file " + path.string() + " is void");
			return result;
		}
		stream.seekg(0, std::ios_base::beg);
		result.resize(filesize);
		stream.read(result.data(), filesize);
		return result;
	}
}