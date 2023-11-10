#pragma once

#include "Age/Math/vec3.hpp"
#include "Age/Math/mat4.hpp"

namespace a_game_engine
{
	struct Transform
	{
	private:
		vec3 _position, _rotation, _scale = { 1.f, 1.f, 1.f };
		mutable mat4 _local, _world;
		mutable bool _localIsChanged = true, _worldIsChanged = true, _otherIsChanged = true;
		bool _isCamera = false;
		const Transform* _parent = nullptr;

		inline void markForUpdate()
		{
			_localIsChanged =
				_worldIsChanged =
				_otherIsChanged = true;
		}
	public:
		Transform(const Transform* parent);

		inline void markParent() const { _worldIsChanged = true; }
		
		vec3& changePosition();
		vec3& changeRotation();
		vec3& changeScale();
		inline const vec3& getPosition() const { return _position; }
		inline const vec3& getRotation() const { return _rotation; }
		inline const vec3& getScale() const { return _scale; }
		void setIsCamera(bool v = true);
		inline bool isCamera() const { return _isCamera; }

		inline void setChangedFlag(bool value = true) { _otherIsChanged = value; }
		inline bool getChangedFlag() const { return _otherIsChanged; }
		const mat4& getLocal() const;
		const mat4& getWorld() const;

		void UNSAFE_setLocalMatrix(const mat4& mat);

		friend class Node;
	};
}
