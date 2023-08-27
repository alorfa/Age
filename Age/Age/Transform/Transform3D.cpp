#include "Transform3D.hpp"
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
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
	const mat4& Transform3D::getMatrix() const
	{
		if (_needToUpdate)
		{
			_needToUpdate = false;
			if (_isCamera)
				_matrix.setViewMatrix(_position, _rotation);
			else
				_matrix.setModelMatrix(_position, _rotation, _scale);
		}
		return _matrix;
	}
}