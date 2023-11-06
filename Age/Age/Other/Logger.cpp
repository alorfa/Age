#include "Logger.hpp"
#include <iostream>

namespace a_game_engine
{
	void Logger::logDebug(const std::string& str)
	{
		std::cout << "Debug: " << str << std::endl;
	}
	void Logger::logInfo(const std::string& str)
	{
		std::cout << "Info: " << str << std::endl;
	}
	void Logger::logError(const std::string& str)
	{
		std::cout << "ERROR: " << str << std::endl;
	}
	void Logger::logWarning(const std::string& str)
	{
		std::cout << "Warning: " << str << std::endl;
	}
}