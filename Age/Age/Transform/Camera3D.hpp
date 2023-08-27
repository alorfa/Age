#pragma once

#include "Transform3D.hpp"
#include "Age/Math/vec2.hpp"

namespace a_game_engine
{
	class Camera3D
	{
		mutable mat4 _projection;
		mutable bool _needUpdate = true;

		float _viewport = 1.f;
		float _aspectRatio = 1.f;
		float _fov = 0.f;
		vec2 _nearFar;
		bool _isOrtho = false;
	public:
		Transform3D transform;

		Camera3D();

		void setFov(float fov); //only for perspective
		void setViewport(float viewport); //only for ortho
		void setAspectRatio(uvec2 windowSize);
		void setAspectRatio(float ratio);
		void setNearFar(vec2 nearFar);
		void setOrtho(bool value = true);

		inline float getFov() const { return _fov; }
		inline float getViewport() const { return _viewport; }
		inline float getAspectRatio() const { return _aspectRatio; }
		inline vec2 getNearFar() const { return _nearFar; }
		inline bool isOrtho() const { return _isOrtho; }
		const mat4& getProjection() const;
	};
}
