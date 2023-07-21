#pragma once

#include "Light.hpp"
#include "Age/Object/Object3D.hpp"

namespace a_game_engine
{
	class PointLightSource : public Object3D
	{
	public:
		PointLight light;

		PointLightSource(Node3D* parent);

		void update(float delta) override;
		void draw(const mat4& parent, const Node3D& scene, const Camera3D& c,
			const Shader* s = nullptr) const override;
	};

	class SpotLightSource : public Object3D
	{
	public:
		SpotLight light;

		SpotLightSource(Node3D* parent);

		void update(float delta) override;
		void draw(const mat4& parent, const Node3D& scene, const Camera3D& c,
			const Shader* s = nullptr) const override;
	};

	class DirLightSource : public Node3D
	{
	public:
		DirLight light;

		DirLightSource(Node3D* parent);
	};
}