#pragma once

#include "vec3.hpp"

namespace a_game_engine
{
	struct mat4
	{
		float data[4][4];

		mat4();

		mat4& operator*=(const mat4& m2);
		mat4 operator*(const mat4& m2) const;

		void reflect();

		void setPerspective(float fov, float aspectRatio, float near, float far);
		void setOrtho(float viewport, float aspectRatio, float near, float far);

		void setModelMatrix(const vec3& position, const vec3& euler, const vec3& scale);
		void setViewMatrix(const vec3& position, const vec3& euler);
		void setViewMatrix(const vec3& offset, const vec3& forward, const vec3& right, const vec3& up);
		void setViewMatrix(const vec3& offset, const vec3& forward, const vec3& up);
		mat4 new_inversed() const;

		void reset();
	};
}