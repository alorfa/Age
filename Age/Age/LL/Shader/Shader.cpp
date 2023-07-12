#include "Shader.hpp"
#include "Age/LL/opengl.h"
#include <iostream>
#include "Age/LL/Texture/Texture2D.hpp"
#include "Age/Math/mat3.hpp"
#include "Age/Math/mat4.hpp"
#include "Age/Resource/Logger.hpp"
#include "Age/Object/Material.hpp"
#include <format>
#include "Age/Scene/Scene3D.hpp"

namespace a_game_engine
{
	using namespace std::string_literals;

	void Shader::destroy()
	{
		if (_id)
			glDeleteProgram(_id);
		_id = 0;
	}
	Shader::Shader(uint vert, uint frag)
	{
		create(vert, frag);
	}
	Shader::Shader(Shader&& other) noexcept
	{
		std::swap(other._id, _id);
	}
	Shader& Shader::operator=(Shader&& other) noexcept
	{
		std::swap(other._id, _id);
		return *this;
	}
	Shader::~Shader()
	{
		destroy();
	}
	void Shader::create(uint vert, uint frag)
	{
		_id = glCreateProgram();
		glAttachShader(_id, vert);
		glAttachShader(_id, frag);
		glLinkProgram(_id);

		GLint log_length = 0;
		GLint link_status = GL_FALSE;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &log_length);
		glGetProgramiv(_id, GL_LINK_STATUS, &link_status);

		if (log_length > 0)
		{
			std::string shader_message;
			shader_message.resize(log_length);
			glGetProgramInfoLog(_id, log_length, nullptr, &shader_message[0]);

			if (link_status == GL_FALSE)
			{
				Logger::logError(shader_message);
			}
			else
			{
				Logger::logWarning(shader_message);
			}
		}
		glValidateProgram(_id);

		glDetachShader(_id, vert);
		glDetachShader(_id, frag);

		glDeleteShader(vert);
		glDeleteShader(frag);
	}
	bool Shader::isValid() const
	{
		int result;
		glGetProgramiv(_id, GL_VALIDATE_STATUS, &result);
		return (bool)result;
	}
	int Shader::location(const char* name) const
	{
		return glGetUniformLocation(_id, name);
	}
	void Shader::setUniform(const char* name, int value) const
	{
		glUniform1i(location(name), value);
	}
	void Shader::setUniform(const char* name, float value) const
	{
		glUniform1f(location(name), value);
	}
	void Shader::setUniform(const char* name, vec2 value) const
	{
		glUniform2f(location(name), value.x, value.y);
	}
	void Shader::setUniform(const char* name, vec3 value) const
	{
		glUniform3f(location(name), value.x, value.y, value.z);
	}
	void Shader::setUniform(const char* name, const mat3& value) const
	{
		glUniformMatrix3fv(location(name), 1, GL_FALSE, value.data);
	}
	void Shader::setUniform(const char* name, const mat4& value) const
	{
		glUniformMatrix4fv(location(name), 1, GL_FALSE, value.data);
	}
	void Shader::setUniform(const char* name, const DirLight& light) const
	{
		setUniform((name + ".dir"s).c_str(), light.dir);
		setUniform((name + ".ambient"s).c_str(), light.ambient);
		setUniform((name + ".color"s).c_str(), light.color);
	}
	void Shader::setUniform(const char* name, const PointLight& light) const
	{
		setUniform((name + ".pos"s).c_str(), light.pos);
		setUniform((name + ".ambient"s).c_str(), light.ambient);
		setUniform((name + ".color"s).c_str(), light.color);
		setUniform((name + ".constant"s).c_str(), light.constant);
		setUniform((name + ".linear"s).c_str(), light.linear);
		setUniform((name + ".quadratic"s).c_str(), light.quadratic); 
	}
	void Shader::setUniform(const char* name, const SpotLight& light) const
	{
		setUniform((name + ".pos"s).c_str(), light.pos);
		setUniform((name + ".dir"s).c_str(), light.dir);
		setUniform((name + ".cutOff"s).c_str(), cosf(light.cutOff));
		setUniform((name + ".outerCutOff"s).c_str(), cosf(light.outerCutOff));
		setUniform((name + ".ambient"s).c_str(), light.ambient);
		setUniform((name + ".color"s).c_str(), light.color);
		setUniform((name + ".constant"s).c_str(), light.constant);
		setUniform((name + ".linear"s).c_str(), light.linear);
		setUniform((name + ".quadratic"s).c_str(), light.quadratic);
	}
	void Shader::setUniform(const char* name, const ColorMaterial& mat) const
	{
		setUniform((name + ".ambient"s).c_str(), mat.ambient);
		setUniform((name + ".diffuse"s).c_str(), mat.diffuse);
		setUniform((name + ".specular"s).c_str(), mat.specular);
		setUniform((name + ".shininess"s).c_str(), mat.shininess);
	}
	void Shader::setLights(const Scene3D& scene) const
	{
		setUniform("sceneAmbient", scene.ambient);
		int i = 0;
		for (const auto& light : scene.pointLights)
		{
			setUniform(("pointLightSources[" + std::to_string(i++) + ']').c_str(), light->light);
			if (i >= MAX_LIGHT_SOURCES)
				break;
		}
		setUniform("pointLightsCount", i);
		i = 0;
		for (const auto& light : scene.dirLights)
		{
			setUniform(("dirLightSources[" + std::to_string(i++) + ']').c_str(), light);
			if (i >= MAX_LIGHT_SOURCES)
				break;
		}
		setUniform("dirLightsCount", i);
		i = 0;
		for (const auto& light : scene.spotLights)
		{
			setUniform(("spotLightSources[" + std::to_string(i++) + ']').c_str(), light->light);
			if (i >= MAX_LIGHT_SOURCES)
				break;
		}
		setUniform("spotLightsCount", i);
	}
	void Shader::setCamera(const Camera3D& camera) const
	{
		setUniform("view", camera.transform.getMatrix());
		setUniform("projection", camera.getProjection());
		setUniform("cameraPos", camera.transform.getPosition());
	}
	uint Shader::setUniform(const char* name, const TextureMaterial& mat, uint sampler) const
	{
		for (size_t i = 0; i < mat.textures.size(); i++)
		{
			if (sampler >= 16)
				break;

			setUniform(std::format("{}[{}]", name, sampler).c_str(), *mat.textures[i], sampler);
			sampler++;
		}

		return sampler;
	}
	void Shader::setUniform(const char* name, const Texture2D& value, uint number) const
	{
		value.activate(number);
		glUniform1i(location(name), (int)number);
	}
	void Shader::use() const
	{
		glUseProgram(_id);
	}
	void Shader::use(uint id)
	{
		glUseProgram(id);
	}
}