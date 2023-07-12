#include "Transform3D.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "Age/Auxiliary/print_math.hpp"

namespace a_game_engine
{
	namespace
	{
		glm::vec3& asGlm(vec3& v)
		{
			return reinterpret_cast<glm::vec3&>(v);
		}
		glm::mat4& asGlm(mat4& m) 
		{
			return reinterpret_cast<glm::mat4&>(m);
		}
		const glm::vec3& asGlm(const vec3& v)
		{
			return reinterpret_cast<const glm::vec3&>(v);
		}
		const glm::mat4& asGlm(const mat4& m)
		{
			return reinterpret_cast<const glm::mat4&>(m);
		}
	}

	Transform3D::Transform3D()
	{
	}
	vec3& Transform3D::changePosition()
	{
		_needToUpdate = true;
		return _position;
	}
	vec3& Transform3D::changeRotation()
	{
		_needToUpdate = true;
		return _rotation;
	}
	vec3& Transform3D::changeScale()
	{
		_needToUpdate = true;
		return _scale;
	}
	void Transform3D::setIsCamera(bool v)
	{
		if (_isCamera != v)
		{
			_isCamera = v;
			_needToUpdate = true;
		}
	}
	mat4 Transform3D::createMatrix(const vec3& position, const vec3& euler, const vec3& scale)
	{
		mat4 result;
		auto& res = asGlm(result);
		res = glm::translate(res, asGlm(position));
		res *= glm::eulerAngleXYZ(euler.x, euler.y, euler.z);
		res = glm::scale(res, asGlm(scale));
		return result;
	}
	mat4 Transform3D::createCameraMatrix(const vec3& position, const vec3& euler, const vec3& scale)
	{
		mat4 result;
		asGlm(result) = glm::lookAt(asGlm(position), 
			asGlm(position + Transform3D::getForwardDir(euler)), glm::vec3(0, 0, 1));
		return result;
	}
	const mat4& Transform3D::getMatrix() const
	{
		if (_needToUpdate)
		{
			_needToUpdate = false;
			if (_isCamera)
				_matrix = createCameraMatrix(_position, _rotation, _scale);
			else
				_matrix = createMatrix(_position, _rotation, _scale);
		}
		return _matrix;
	}
	vec3 Transform3D::getForwardDir(const vec3& euler)
	{
		float cosx, cosy, sinx, siny;
		cosx = glm::cos(euler.x);
		cosy = glm::cos(euler.y);
		sinx = glm::sin(euler.x);
		siny = glm::sin(euler.y);
		return { -sinx * siny, sinx * cosy, -cosx };
	}
	vec3 Transform3D::getRightDir(const vec3& euler)
	{
		float cosz, cosy, sinz, siny;
		cosz = glm::cos(euler.z);
		cosy = glm::cos(euler.y);
		sinz = glm::sin(euler.z);
		siny = glm::sin(euler.y);
		return { cosz * cosy, siny * cosz, sinz };
	}
	vec3 Transform3D::getUpDir(const vec3& euler)
	{
		float cosz, cosx, sinz, sinx;
		cosz = glm::cos(euler.z);
		cosx = glm::cos(euler.x);
		sinz = glm::sin(euler.z);
		sinx = glm::sin(euler.x);
		return { 0, 0, 0 };
	}
}