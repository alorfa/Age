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
	ShaderProgram::ShaderProgram(const ShaderModule& vert, const ShaderModule& frag)
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
	void ShaderProgram::create(const ShaderModule& vert, const ShaderModule& frag)
	{
		destroy();
		_id = glCreateProgram();
		glAttachShader(_id, vert.getId());
		glAttachShader(_id, frag.getId());
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

		glDetachShader(_id, vert.getId());
		glDetachShader(_id, frag.getId());
	}
	bool ShaderProgram::isValid() const
	{
		return _isValid;
	}
	int ShaderProgram::getLocation(const char* name) const
	{
		int result = glGetUniformLocation(_id, name);
		return result;
	}
	void ShaderProgram::setUniform(int location, int value) const
	{
		glUniform1i(location, value);
	}
	void ShaderProgram::setUniform(int location, float value) const
	{
		glUniform1f(location, value);
	}
	void ShaderProgram::setUniform(int location, vec2 value) const
	{
		glUniform2f(location, value.x, value.y);
	}
	void ShaderProgram::setUniform(int location, vec3 value) const
	{
		glUniform3f(location, value.x, value.y, value.z);
	}
	void ShaderProgram::setUniform(int location, const mat3& value) const
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, value.data);
	}
	void ShaderProgram::setUniform(int location, const mat4& value) const
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, (const float*)value.data);
	}
	void ShaderProgram::setUniform(const char* name, const DirLight& light) const
	{
		setUniform(getLocation((name + ".dir"s).c_str()), light.dir);
		setUniform(getLocation((name + ".ambient"s).c_str()), light.ambient);
		setUniform(getLocation((name + ".color"s).c_str()), light.color);
	}
	void ShaderProgram::setUniform(const char* name, const PointLight& light) const
	{
		setUniform(getLocation((name + ".pos"s).c_str()), light.pos);
		setUniform(getLocation((name + ".ambient"s).c_str()), light.ambient);
		setUniform(getLocation((name + ".color"s).c_str()), light.color);
		setUniform(getLocation((name + ".constant"s).c_str()), light.constant);
		setUniform(getLocation((name + ".linear"s).c_str()), light.linear);
		setUniform(getLocation((name + ".quadratic"s).c_str()), light.quadratic);
	}
	void ShaderProgram::setUniform(const char* name, const SpotLight& light) const
	{
		setUniform(getLocation((name + ".pos"s).c_str()), light.pos);
		setUniform(getLocation((name + ".dir"s).c_str()), light.dir);
		setUniform(getLocation((name + ".cutOff"s).c_str()), cosf(light.cutOff));
		setUniform(getLocation((name + ".outerCutOff"s).c_str()), cosf(light.outerCutOff));
		setUniform(getLocation((name + ".ambient"s).c_str()), light.ambient);
		setUniform(getLocation((name + ".color"s).c_str()), light.color);
		setUniform(getLocation((name + ".constant"s).c_str()), light.constant);
		setUniform(getLocation((name + ".linear"s).c_str()), light.linear);
		setUniform(getLocation((name + ".quadratic"s).c_str()), light.quadratic);
	}
	void ShaderProgram::setLights(const Node3D& scene) const
	{
		int pointLightsCount = 0;
		int dirLightsCount = 0;
		int spotLightsCount = 0;
		for (const auto& light : scene.infChildren)
		{
			auto* pointLight = light->as<PointLightSource>();
			if (pointLight)
			{
				//std::format("pointLightSources[{}]", pointLightsCount++).c_str()
				setUniform(("pointLightSources[" + std::to_string(pointLightsCount++) + ']').c_str(), pointLight->light);
				continue;
			}
			auto* dirLight = light->as<DirLightSource>();
			if (dirLight)
			{
				setUniform(("dirLightSources[" + std::to_string(dirLightsCount++) + ']').c_str(), dirLight->light);
				continue;
			}
			auto* spotLight = light->as<SpotLightSource>();
			if (spotLight)
			{
				setUniform(("spotLightSources[" + std::to_string(spotLightsCount++) + ']').c_str(), spotLight->light);
				continue;
			}
		}
		setUniform(getLocation("pointLightsCount"), pointLightsCount);
		setUniform(getLocation("dirLightsCount"), dirLightsCount);
		setUniform(getLocation("spotLightsCount"), spotLightsCount);
	}
	void ShaderProgram::setCamera(const Camera3D& camera) const
	{
		setUniform(getLocation("view"), camera.transform.getMatrix());
		setUniform(getLocation("projection"), camera.getProjection());
		setUniform(getLocation("cameraPos"), camera.transform.getPosition());
	}
	uint ShaderProgram::setUniform(const char* name, const TextureMaterial& mat, uint sampler) const
	{
		for (size_t i = 0; i < mat.textures.size(); i++)
		{
			if (sampler >= 16)
				break;

			setUniform(getLocation(std::format("{}[{}]", name, sampler).c_str()), *mat.textures[i], sampler);
			sampler++;
		}

		return sampler;
	}
	void ShaderProgram::setUniform(int location, const Texture2D& value, uint number) const
	{
		value.activate(number);
		glUniform1i(location, (int)number);
	}
	void ShaderProgram::setUniform(int location, const CubeMap& value, uint number) const
	{
		value.activate(number);
		glUniform1i(location, (int)number);
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