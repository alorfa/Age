#pragma once

#include "Age/Object/Component.hpp"
#include "Age/Light/Light.hpp"

namespace a_game_engine
{
	class PointLightComponent : public Component
	{
		PointLight _light;
	public:
		const Node* node = nullptr;

		inline PointLightComponent(const Node& n)
			: node(&n) {}

		void setColor(const vec3& color);
		void setAmbient(const vec3& color);
		inline const PointLight& getLight() const { return _light; }

		void update(float delta) override;
	};

	class SpotLightComponent : public Component
	{
		SpotLight _light;
	public:
		const Node* node = nullptr;

		inline SpotLightComponent(const Node& n)
			: node(&n) {}

		void setColor(const vec3& color);
		void setAmbient(const vec3& color);
		inline const SpotLight& getLight() const { return _light; }

		void update(float delta) override;
	};

	class DirLightComponent : public Component
	{
	public:
		DirLight light;
	};
}