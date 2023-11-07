#include "LightComponents.hpp"
#include "Age/Object/Node.hpp"
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	void PointLightComponent::update(float delta)
	{
		_light.pos = node->getTransform().getPosition(); //TODO: use world matrix(all the light types)
	}
	void PointLightComponent::setColor(const vec3& color)
	{
		_light.color = color;
		//TODO: use the model and set the emission color(all the light types)
	}
	void PointLightComponent::setAmbient(const vec3& color)
	{
		_light.ambient = color;
	}
	void SpotLightComponent::update(float delta)
	{
		_light.pos = node->getTransform().getPosition();
		_light.dir = Math::getForwardDir(node->getTransform().getRotation());
	}
	void SpotLightComponent::setColor(const vec3& color)
	{
		_light.color = color;
	}
	void SpotLightComponent::setAmbient(const vec3& color)
	{
		_light.ambient = color;
	}
}
