#include "LightComponents.hpp"
#include "Age/Object/Node.hpp"
#include "Age/Math/Math.hpp"
#include "MeshComponent.hpp"

namespace a_game_engine
{
	PointLightComponent& PointLightComponent::addModel(Node& n)
	{
		n.forEach([this](Node& node) {
			auto meshes = node.findAllComponents<MeshComponent>();
			for (auto* m : meshes)
				emissionMeshes.push_back(m);
			});
		return *this;
	}
	void PointLightComponent::update(float delta)
	{
		_light.pos = node->getTransform().getPosition(); //TODO: use world matrix(all the light types)
		for (auto& m : emissionMeshes)
			m->mesh.material.setValue("emission", ShaderProperty(_light.color));
	}
	PointLightComponent& PointLightComponent::setColor(const vec3& color, float ambientRatio)
	{
		_light.ambient = color * ambientRatio;
		_light.color = color - _light.ambient;
		return *this;
	}
	PointLightComponent& PointLightComponent::setDirect(const vec3& color)
	{
		_light.color = color;
		return *this;
	}
	PointLightComponent& PointLightComponent::setAmbient(const vec3& color)
	{
		_light.ambient = color;
		return *this;
	}
	void SpotLightComponent::update(float delta)
	{
		_light.pos = node->getTransform().getPosition();
		_light.dir = Math::getForwardDir(node->getTransform().getRotation());
	}
	SpotLightComponent& SpotLightComponent::setColor(const vec3& color, float ambientRatio)
	{
		_light.ambient = color * ambientRatio;
		_light.color = color - _light.ambient;
		return *this;
	}
	SpotLightComponent& SpotLightComponent::setDirect(const vec3& color)
	{
		_light.color = color;
		return *this;
	}
	SpotLightComponent& SpotLightComponent::setAmbient(const vec3& color)
	{
		_light.ambient = color;
		return *this;
	}
}
