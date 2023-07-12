#pragma once

#include "Transform3D.hpp"
#include "Age/Math/vec2.hpp"

namespace a_game_engine
{
	class Camera3D
	{
		mutable mat4 _projection;
		mutable bool _needUpdate = true;
		mutable mat4 _projectView;
		mutable bool _needUpdatePV = true;

		uvec2 _windowSize;
		float _fov = 0.f;
		vec2 _nearFar;
		bool _isOrtho = false;

		mat4 createProjectionMatrix() const;
	public:
		Transform3D transform;

		Camera3D();

		void setFov(float fov);
		void setWindowSize(uvec2 windowSize);
		void setNearFar(vec2 nearFar);
		void setOrtho(bool value = true);

		inline float getFov() { return _fov; }
		inline uvec2 getWindowSize() { return _windowSize; }
		inline vec2 getNearFar() { return _nearFar; }
		inline bool isOrtho() { return _isOrtho; }
		const mat4& getProjection() const;
		const mat4& getPVMatrix() const;
	};
}
