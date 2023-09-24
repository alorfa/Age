#include "ShaderModule.hpp"
#include "Age/LL/opengl.h"
#include "Age/Resource/Logger.hpp"

namespace a_game_engine
{
	namespace
	{
		GLenum toOglType(int type)
		{
			switch (type)
			{
			case ShaderModule::Vertex:
				return GL_VERTEX_SHADER;
			case ShaderModule::Fragment:
				return GL_FRAGMENT_SHADER;
			default:
				return GL_VERTEX_SHADER;
			}
		}
	}

	const std::string ShaderModule::glslVersionString = "#version 330 core\n";

	void ShaderModule::create(const std::string& data, Type type)
	{
		destroy();
		std::string resultData = glslVersionString + data;
		const char* toGlData = resultData.c_str();
		_id = glCreateShader(toOglType(type));
		glShaderSource(_id, 1, &toGlData, nullptr);
		glCompileShader(_id);

		GLint log_length = 0;
		GLint compile_status = GL_FALSE;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &log_length);
		glGetShaderiv(_id, GL_COMPILE_STATUS, &compile_status);

		if (log_length > 0)
		{
			std::string shader_message;
			shader_message.resize(log_length);
			glGetShaderInfoLog(_id, log_length, nullptr, &shader_message[0]);

			if (compile_status == GL_FALSE)
			{
				//Logger::logError(shader_message);
				destroy();
			}
			else
			{
				Logger::logWarning(shader_message);
			}
		}
	}
	ShaderModule::ShaderModule(const std::string& data, Type type)
	{
		create(data, type);
	}
	ShaderModule::ShaderModule()
	{
	}
	ShaderModule::~ShaderModule()
	{
		destroy();
	}
	void ShaderModule::destroy()
	{
		if (_id)
		{
			glDeleteShader(_id);
			_id = 0;
		}
	}
}