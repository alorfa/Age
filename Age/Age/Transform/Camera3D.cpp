#include "Camera3D.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace a_game_engine
{
	Camera3D::Camera3D()
	{
		transform.setIsCamera();
	}

	mat4 Camera3D::createProjectionMatrix() const
	{
		mat4 result;
		auto& res = reinterpret_cast<glm::mat4&>(result);
		if (_isOrtho)
		{
			res = glm::ortho(-1.f, -1.f, 1.f, 1.f,
				_nearFar.x, _nearFar.y);
		}
		else
		{
			res = glm::perspective(_fov, 
				(float)_windowSize.x / (float)_windowSize.y, _nearFar.x, _nearFar.y);
		}
		return result;
	}
	void Camera3D::setFov(float fov)
	{
		_fov = fov;
		_needUpdate = true;
	}
	void Camera3D::setWindowSize(uvec2 windowSize)
	{
		_windowSize = windowSize;
		_needUpdate = true;
	}
	void Camera3D::setNearFar(vec2 nearFar)
	{
		_nearFar = nearFar;
		_needUpdate = true;
	}
	void Camera3D::setOrtho(bool value)
	{
		_isOrtho = value;
		_needUpdate = true;
	}
	const mat4& Camera3D::getProjection() const
	{
		if (_needUpdate)
		{
			_needUpdate = false;
			_needUpdatePV = true;
			_projection = createProjectionMatrix();
		}
		return _projection;
	}
	const mat4& Camera3D::getPVMatrix() const
	{
		return _projectView = getProjection() * transform.getMatrix();
	}
}
