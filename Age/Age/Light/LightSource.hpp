#pragma once

#include "Light.hpp"
#include "Age/Object/Object3D.hpp"

namespace a_game_engine
{
	class Scene3D;

	class PointLightSource : public Object3D
	{
	public:
		PointLight light;

		PointLightSource(Scene3D& scene, Node3D* parent);

		void draw(const mat4& parent, const Camera3D& c,
			const Shader* s = nullptr) const override;
	};

	class SpotLightSource : public Object3D
	{
	public:
		SpotLight light;

		SpotLightSource(Scene3D& scene, Node3D* parent);

		void draw(const mat4& parent, const Camera3D& c,
			const Shader* s = nullptr) const override;
	};

	class DirLightSource : public Node3D
	{
	public:
		DirLight light;

		DirLightSource(Scene3D& scene, Node3D* parent);
	};
}