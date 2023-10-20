#include "Transform3D.hpp"
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	Transform3D::Transform3D(const Transform3D* parent)
		: _parent(parent)
	{
	}
	vec3& Transform3D::changePosition()
	{
		markForUpdate();
		return _position;
	}
	vec3& Transform3D::changeRotation()
	{
		markForUpdate();
		return _rotation;
	}
	vec3& Transform3D::changeScale()
	{
		markForUpdate();
		return _scale;
	}
	void Transform3D::setIsCamera(bool v)
	{
		if (_isCamera != v)
		{
			_isCamera = v;
			markForUpdate();
		}
	}
	
	const mat4& Transform3D::getWorld() const
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
	const mat4& Transform3D::getLocal() const
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