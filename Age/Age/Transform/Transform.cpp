#include "Transform.hpp"
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	vec3& Transform::changePosition()
	{
		markForUpdate();
		return _position;
	}
	vec3& Transform::changeRotation()
	{
		markForUpdate();
		return _rotation;
	}
	vec3& Transform::changeScale()
	{
		markForUpdate();
		return _scale;
	}
	void Transform::setIsCamera(bool v)
	{
		if (_isCamera != v)
		{
			_isCamera = v;
			markForUpdate();
		}
	}
	
	const mat4& Transform::getWorld() const
	{
		if (_parent == nullptr)
			return getLocal();

		if (_isCamera) //TODO: use parent matrix
			return getLocal(); 

		if (_worldIsChanged)
		{
			_worldIsChanged = false;

			const auto& parentMatrix = _parent->getWorld();
			_world = parentMatrix * getLocal(); //TODO: use fast multiplication
		}
		return _world;
	}
	void Transform::UNSAFE_setLocalMatrix(const mat4& mat)
	{
		_local = mat;
		_localIsChanged = false;
	}
	const mat4& Transform::getLocal() const
	{
		if (_localIsChanged)
		{
			_localIsChanged = false;
			if (_isCamera)
				_local.setViewMatrix(_position, _rotation);
			else
				_local.setModelMatrix(_position, _rotation, _scale);
		}
		return _local;
	}
}