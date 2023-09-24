#pragma once

#include "Age/types.hpp"
#include <string>

namespace a_game_engine
{
	class ShaderModule
	{
		uint _id = 0;
	public:
		enum Type { Undefined, Vertex, Fragment };

		static const std::string glslVersionString;

		ShaderModule(const std::string& data, Type type);
		ShaderModule();
		~ShaderModule();
		void create(const std::string& data, Type type);
		void destroy();
		inline uint getId() const { return _id; }
		inline bool isValid() const { return _id != 0; }
	};
}