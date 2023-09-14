#include "ShaderProgram.hpp"
#include "Age/LL/opengl.h"
#include <iostream>
#include "Age/LL/Texture/Texture2D.hpp"
#include "Age/LL/Texture/CubeMap.hpp"
#include "Age/Math/mat3.hpp"
#include "Age/Math/mat4.hpp"
#include "Age/Resource/Logger.hpp"
#include "Age/Object/Material.hpp"
#include "Age/Object/Node3D.hpp"
#include "Age/Transform/Camera3D.hpp"
#include "Age/Light/LightSource.hpp"
#include <format>

namespace a_game_engine
{
	using namespace std::string_literals;

	void ShaderProgram::destroy()
	{
		if (_id)
			glDeleteProgram(_id);
		_id = 0;
	}
	ShaderProgram::ShaderProgram(uint vert, uint frag)
	{
		create(vert, frag);
	}
	ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	{
		std::swap(other._id, _id);
	}
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
	{
		std::swap(other._id, _id);
		return *this;
	}
	ShaderProgram::~ShaderProgram()
	{
		destroy();
	}
	void ShaderProgram::create(uint vert, uint frag)
	{
		destroy();
		_id = glCreateProgram();
		glAttachShader(_id, vert);
		glAttachShader(_id, frag);
		glLinkProgram(_id);

		GLint log_length = 0;
		GLint link_status = GL_FALSE;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &log_length);
		glGetProgramiv(_id, GL_LINK_STATUS, &link_status);

		_isValid = true;

		if (log_length > 0)
		{
			std::string shader_message;
			shader_message.resize(log_length);
			glGetProgramInfoLog(_id, log_length, nullptr, &shader_message[0]);

			if (link_status == GL_FALSE)
			{
				_isValid = false;
				Logger::logError(shader_message);
			}
			else
			{
				Logger::logWarning(shader_message);
			}
		}

		glDetachShader(_id, vert);
		glDetachShader(_id, frag);

		glDeleteShader(vert);
		glDeleteShader(frag);
	}
	bool ShaderProgram::isValid() const
	{
		return _isValid;
	}
	int ShaderProgram::location(const char* name) const
	{
		return glGetUniformLocation(_id, name);
	}
	void ShaderProgram::setUniform(const char* name, int value) const
	{
		glUniform1i(location(name), value);
	}
	void ShaderProgram::setUniform(const char* name, float value) const
	{
		glUniform1f(location(name), value);
	}
	void ShaderProgram::setUniform(const char* name, vec2 value) const
	{
		glUniform2f(location(name), value.x, value.y);
	}
	void ShaderProgram::setUniform(const char* name, vec3 value) const
	{
		glUniform3f(location(name), value.x, value.y, value.z);
	}
	void ShaderProgram::setUniform(const char* name, const mat3& value) const
	{
		glUniformMatrix3fv(location(name), 1, GL_FALSE, value.data);
	}
	void ShaderProgram::setUniform(const char* name, const mat4& value) const
	{
		glUniformMatrix4fv(location(name), 1, GL_FALSE, (const float*)value.data);
	}
	void ShaderProgram::setUniform(const char* name, const DirLight& light) const
	{
		setUniform((name + ".dir"s).c_str(), light.dir);
		setUniform((name + ".ambient"s).c_str(), light.ambient);
		setUniform((name + ".color"s).c_str(), light.color);
	}
	void ShaderProgram::setUniform(const char* name, const PointLight& light) const
	{
		setUniform((name + ".pos"s).c_str(), light.pos);
		setUniform((name + ".ambient"s).c_str(), light.ambient);
		setUniform((name + ".color"s).c_str(), light.color);
		setUniform((name + ".constant"s).c_str(), light.constant);
		setUniform((name + ".linear"s).c_str(), light.linear);
		setUniform((name + ".quadratic"s).c_str(), light.quadratic); 
	}
	void ShaderProgram::setUniform(const char* name, const SpotLight& light) const
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
	void ShaderProgram::setUniform(const char* name, const ColorMaterial& mat) const
	{
		setUniform((name + ".ambient"s).c_str(), mat.ambient);
		setUniform((name + ".diffuse"s).c_str(), mat.diffuse);
		setUniform((name + ".specular"s).c_str(), mat.specular);
		setUniform((name + ".shininess"s).c_str(), mat.shininess);
	}
	void ShaderProgram::setLights(const Node3D& scene) const
	{
		int pointLightsCount = 0;
		int dirLightsCount = 0;
		int spotLightsCount = 0;
		for (const auto& light : scene.infChildren)
		{
			auto* pointLight = light->as<PointLightSource>();
			if (pointLight and pointLightsCount < MAX_LIGHT_SOURCES)
			{
				//std::format("pointLightSources[{}]", pointLightsCount++).c_str()
				setUniform(("pointLightSources[" + std::to_string(pointLightsCount++) + ']').c_str(), pointLight->light);
				continue;
			}
			auto* dirLight = light->as<DirLightSource>();
			if (dirLight and dirLightsCount < MAX_LIGHT_SOURCES)
			{
				setUniform(("dirLightSources[" + std::to_string(dirLightsCount++) + ']').c_str(), dirLight->light);
				continue;
			}
			auto* spotLight = light->as<SpotLightSource>();
			if (spotLight and spotLightsCount < MAX_LIGHT_SOURCES)
			{
				setUniform(("spotLightSources[" + std::to_string(spotLightsCount++) + ']').c_str(), spotLight->light);
				continue;
			}
		}
		setUniform("pointLightsCount", pointLightsCount);
		setUniform("dirLightsCount", dirLightsCount);
		setUniform("spotLightsCount", spotLightsCount);
	}
	void ShaderProgram::setCamera(const Camera3D& camera) const
	{
		setUniform("view", camera.transform.getMatrix());
		setUniform("projection", camera.getProjection());
		setUniform("cameraPos", camera.transform.getPosition());
	}
	uint ShaderProgram::setUniform(const char* name, const TextureMaterial& mat, uint sampler) const
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
	void ShaderProgram::setUniform(const char* name, const Texture2D& value, uint number) const
	{
		value.activate(number);
		glUniform1i(location(name), (int)number);
	}
	void ShaderProgram::setUniform(const char* name, const CubeMap& value, uint number) const
	{
		value.activate(number);
		glUniform1i(location(name), (int)number);
	}
	void ShaderProgram::use() const
	{
		glUseProgram(_id);
	}
	void ShaderProgram::use(uint id)
	{
		glUseProgram(id);
	}
}