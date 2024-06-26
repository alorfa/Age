#pragma once

#include "Age/Object/Component.hpp"
#include "Age/Light/Light.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"

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
		PointLightComponent& setSize(float size);
		PointLightComponent& setRadius(float radius);
		PointLightComponent& setOffset(float offset);
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
		SpotLightComponent& setSize(float size);
		SpotLightComponent& setRadius(float radius);
		SpotLightComponent& setOffset(float offset);
		SpotLightComponent& setCutOff(float rad);
		SpotLightComponent& setOuterCutOff(float rad);
		inline const SpotLight& getLight() const { return _light; }

		void update(float delta) override;
	};

	class DirLightComponent : public Component
	{
		DirLight light;
		mutable FrameBuffer2D fb;
		float bias = -0.0001f;

		const Node* node = nullptr;
		vec3 prevDir = { 0.f };
	public:
		inline DirLightComponent(const Node& n) : node(&n) {}

		DirLightComponent& setSize(float size);
		DirLightComponent& setDirection(vec3 dir);
		DirLightComponent& setColor(const vec3& color, float ambientRatio);
		DirLightComponent& enableShadow(bool value = true);
		DirLightComponent& createShadowMap(uvec2 size = {512u}, TextureFormat format = TextureFormat::Depth16);
		DirLightComponent& setBias(float bias = 0.0001);

		inline const DirLight& getLight() const { return light; }
		inline const Node& getNode() const { return *node; }
		inline float getBias() const { return bias; }

		void update(float delta) override;

		void drawSceneFromShadow(const Node& rootNode) const;
	};
}