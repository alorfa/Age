#include "Camera.hpp"

namespace a_game_engine
{
	Camera::Camera()
		: transform(nullptr)
	{
		transform.setIsCamera();
	}

	void Camera::setFov(float fov)
	{
		_fov = fov;
		_needUpdate = true;
	}
	void Camera::setViewport(float viewport)
	{
		_viewport = viewport;
		_needUpdate = true;
	}
	void Camera::setAspectRatio(uvec2 windowSize)
	{
		setAspectRatio((float)windowSize.x / (float)windowSize.y);
	}
	void Camera::setAspectRatio(float ratio)
	{
		_aspectRatio = ratio;
		_needUpdate = true;
	}
	void Camera::setNearFar(vec2 nearFar)
	{
		_nearFar = nearFar;
		_needUpdate = true;
	}
	void Camera::setOrtho(bool value)
	{
		_isOrtho = value;
		_needUpdate = true;
	}
	const mat4& Camera::getProjection() const
	{
		if (_needUpdate)
		{
			_needUpdate = false;
			if (_isOrtho)
				_projection.setOrtho(_viewport, _aspectRatio, _nearFar.x, _nearFar.y);
			else
				_projection.setPerspective(_fov, _aspectRatio, _nearFar.x, _nearFar.y);
		}
		return _projection;
	}
}
