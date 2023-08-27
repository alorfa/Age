#pragma once

#include "Age/Math/vec3.hpp"
#include "Age/Math/mat4.hpp"

namespace a_game_engine
{
	struct Transform3D
	{
	private:
		vec3 _position, _rotation, _scale = { 1.f, 1.f, 1.f };
		mutable mat4 _matrix;
		mutable bool _needToUpdate = true;
		bool _isCamera = false;
	public:
		Transform3D();

		vec3& changePosition();
		vec3& changeRotation();
		vec3& changeScale();
		inline const vec3& getPosition() const { return _position; }
		inline const vec3& getRotation() const { return _rotation; }
		inline const vec3& getScale() const { return _scale; }
		void setIsCamera(bool v = true);
		inline bool isCamera() const { return _isCamera; }

		inline bool isNeedToUpdate() const { return _needToUpdate; }
		const mat4& getMatrix() const;
	};
}
