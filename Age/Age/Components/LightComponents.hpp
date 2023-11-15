#pragma once

#include "Age/Object/Component.hpp"
#include "Age/Light/Light.hpp"

namespace a_game_engine
{
	class MeshComponent;
	class Node;

	class PointLightComponent : public Component
	{
		PointLight _light;
	public:
		const Node* node = nullptr;
		std::vector<MeshComponent*> emissionMeshes;

		inline PointLightComponent(const Node& n)
			: node(&n) {}

		PointLightComponent& setColor(const vec3& color, float ambientRatio);
		PointLightComponent& setDirect(const vec3& color);
		PointLightComponent& setAmbient(const vec3& color);
		inline const PointLight& getLight() const { return _light; }

		PointLightComponent& addModel(Node& n);

		void update(float delta) override;
	};

	class SpotLightComponent : public Component
	{
		SpotLight _light;
	public:
		const Node* node = nullptr;

		inline SpotLightComponent(const Node& n)
			: node(&n) {}

		SpotLightComponent& setColor(const vec3& color, float ambientRatio);
		SpotLightComponent& setDirect(const vec3& color);
		SpotLightComponent& setAmbient(const vec3& color);
		inline const SpotLight& getLight() const { return _light; }

		void update(float delta) override;
	};

	class DirLightComponent : public Component
	{
	public:
		DirLight light;

		inline DirLightComponent(const Node& n) {}
		DirLightComponent() = default;
	};
}