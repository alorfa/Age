#include "ShaderProgram.hpp"
#include "Age/LL/opengl.h"
#include <iostream>
#include "Age/LL/Texture/Texture2D.hpp"
#include "Age/LL/Texture/CubeMap.hpp"
#include "Age/Math/mat3.hpp"
#include "Age/Math/mat4.hpp"
#include "Age/Resource/Logger.hpp"
#include "Age/Material/Material.hpp"
#include "Age/Object/Node.hpp"
#include "Age/Transform/Camera.hpp"
#include "Age/Light/LightSource.hpp"
#include "Age/Material/ShaderProps.hpp"

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

		if (log_length > 1) //some videocards can return "\0" with length 1
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
	void ShaderProgram::setCamera(const Camera& camera) const
	{
		setUniform(getLocation("view"), camera.transform.getWorld());
		setUniform(getLocation("projection"), camera.getProjection());
		setUniform(getLocation("cameraPos"), camera.transform.getPosition());
	}
	void ShaderProgram::setTransformProps(const TransformProps& props) const
	{
		setUniform(getLocation("model"), *props.model);
		setUniform(getLocation("view"), *props.view);
		setUniform(getLocation("projection"), *props.proj);
		setUniform(getLocation("cameraPos"), props.cameraPos);
		setUniform(getLocation("near"), props.near);
		setUniform(getLocation("far"), props.far);
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
	void ShaderProgram::setUniform(int location, const ShaderProperty& value) const
	{
		const auto type = value.getType();
		const ShaderProperty::Texture2DProp* tex2D;
		const ShaderProperty::CubeMapProp* cubemap;
		switch (type)
		{
		case ShaderProperty::Type::Int:
			setUniform(location, std::get<int>(value.value)); break;
		case ShaderProperty::Type::Float:
			setUniform(location, std::get<float>(value.value)); break;
		case ShaderProperty::Type::Vec2:
			setUniform(location, std::get<vec2>(value.value)); break;
		case ShaderProperty::Type::Vec3:
			setUniform(location, std::get<vec3>(value.value)); break;
		case ShaderProperty::Type::Mat3:
			setUniform(location, std::get<mat3>(value.value)); break;
		case ShaderProperty::Type::Mat4:
			setUniform(location, std::get<mat4>(value.value)); break;
		case ShaderProperty::Type::Texture2D:
			tex2D = &std::get<ShaderProperty::Texture2DProp>(value.value);
			if (tex2D->slot > -1)
				setUniform(location, *tex2D->texture, tex2D->slot); 
			break;
		case ShaderProperty::Type::CubeMap:
			cubemap = &std::get<ShaderProperty::CubeMapProp>(value.value);
			if (cubemap->slot > -1)
				setUniform(location, *cubemap->cubemap, cubemap->slot);
			break;
		}
	}
	void ShaderProgram::setUniform(const MaterialProperty& value) const
	{
		setUniform(getLocation(value.name.c_str()), value.property);
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