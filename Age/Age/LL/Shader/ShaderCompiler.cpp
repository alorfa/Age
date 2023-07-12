#include "ShaderCompiler.hpp"
#include "Age/LL/opengl.h"
#include "Age/Resource/File.hpp"
#include <iostream>
#include "Age/Resource/Logger.hpp"

namespace a_game_engine
{
	namespace
	{
		GLenum toOglType(int type)
		{
			switch (type)
			{
			case ShaderCompiler::Vertex:
				return GL_VERTEX_SHADER;
			case ShaderCompiler::Fragment:
				return GL_FRAGMENT_SHADER;
			default:
				return GL_VERTEX_SHADER;
			}
		}
	}

	uint ShaderCompiler::loadFromMemory(const char* data, int type)
	{
		uint result = glCreateShader(toOglType(type));
		glShaderSource(result, 1, &data, nullptr);
		glCompileShader(result);

		GLint log_length = 0;
		GLint compile_status = GL_FALSE;
		glGetShaderiv(result, GL_INFO_LOG_LENGTH, &log_length);
		glGetShaderiv(result, GL_COMPILE_STATUS, &compile_status);

		if (log_length > 0)
		{
			std::string shader_message;
			shader_message.resize(log_length);
			glGetShaderInfoLog(result, log_length, nullptr, &shader_message[0]);

			if (compile_status == GL_FALSE)
			{
				Logger::logError(shader_message);
				glDeleteShader(result);
				return 0;
			}
			Logger::logWarning(shader_message);
		}

		return result;
	}
	uint ShaderCompiler::loadFromFile(const std::filesystem::path& path, int type)
	{
		auto shaderText = File::readAllText(path);
		return loadFromMemory(shaderText.c_str(), type);
	}
}