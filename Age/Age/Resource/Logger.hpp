#pragma once

#include <string>

namespace a_game_engine
{
	class Logger
	{
	public:
		static void logDebug(const std::string& str);
		static void logInfo(const std::string& str);
		static void logWarning(const std::string& str);
		static void logError(const std::string& str);
	};
}