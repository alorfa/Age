#pragma once

#include "Light.hpp"
#include "Age/Object/Object3D.hpp"

namespace a_game_engine
{
	class PointLightSource : public Object3D
	{
	public:
		PointLight light;

		PointLightSource(const Model3D& m, const Shader& sh);

		virtual void update(float delta) override;
		virtual void draw(const Scene3D&, const Camera3D&, const Shader* s = nullptr) const override;
	};

	class SpotLightSource : public Object3D
	{
	public:
		SpotLight light;

		SpotLightSource(const Model3D& m, const Shader& sh);

		virtual void update(float delta) override;
		virtual void draw(const Scene3D&, const Camera3D&, const Shader* s = nullptr) const override;
	};
}