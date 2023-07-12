#pragma once

#include "Age/Math/vec2.hpp"
#include "Age/Math/vec3.hpp"
#include "Age/Math/mat3.hpp"
#include "Age/Math/mat4.hpp"
#include <iostream>
#include "DEBUG.hpp"
#include <glm/gtx/quaternion.hpp>

namespace a_game_engine
{
	template <typename T>
	std::ostream& operator<<(std::ostream& stream, vector2<T> v)
	{
		stream << "[" << v.x << ", " << v.y << ']';
		return stream;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& stream, vector3<T> v)
	{
		stream << "[" << v.x << ", " << v.y << ", " << v.z << ']';
		return stream;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& stream, glm::tvec3<T> v)
	{
		stream << "[" << v.x << ", " << v.y << ", " << v.z << ']';
		return stream;
	}

	inline std::ostream& operator<<(std::ostream& stream, const mat3& v)
	{
		stream << "[" << v.data[0] << ", " << v.data[3] << ", " << v.data[6] << ']' << std::endl;
		stream << "[" << v.data[1] << ", " << v.data[4] << ", " << v.data[7] << ']' << std::endl;
		stream << "[" << v.data[2] << ", " << v.data[5] << ", " << v.data[8] << ']' << std::endl;
		return stream;
	}
	inline std::ostream& operator<<(std::ostream& stream, const mat4& v)
	{
		stream << "[" << v.data[0] << ", " << v.data[4] << ", " << v.data[8] << ", " << v.data[12] << ']' << std::endl;
		stream << "[" << v.data[1] << ", " << v.data[5] << ", " << v.data[9] << ", " << v.data[13] << ']' << std::endl;
		stream << "[" << v.data[2] << ", " << v.data[6] << ", " << v.data[10] << ", " << v.data[14] << ']' << std::endl;
		stream << "[" << v.data[3] << ", " << v.data[7] << ", " << v.data[11] << ", " << v.data[15] << ']' << std::endl;
		return stream;
	}

	inline std::ostream& operator<<(std::ostream& stream, const glm::quat& v)
	{
		stream << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']' << std::endl;
		return stream;
	}
}
