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
		//TODO: use world matrix(all the light types)
		//TODO: move to the getLight method?
		_light.pos = node->getTransform().getPosition();
		for (auto& m : emissionMeshes)
			m->mesh.material.setValue("emission", ShaderProperty(_light.color + _light.ambient));
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
	PointLightComponent& PointLightComponent::setSize(float size)
	{
		_light.size = size;
		return *this;
	}
	PointLightComponent& PointLightComponent::setRadius(float radius)
	{
		_light.radius = radius;
		return *this;
	}
	PointLightComponent& PointLightComponent::setOffset(float offset)
	{
		_light.attOffset = offset;
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
	SpotLightComponent& SpotLightComponent::setSize(float size)
	{
		_light.size = size;
		return *this;
	}
	SpotLightComponent& SpotLightComponent::setRadius(float radius)
	{
		_light.radius = radius;
		return *this;
	}
	SpotLightComponent& SpotLightComponent::setOffset(float offset)
	{
		_light.attOffset = offset;
		return *this;
	}
	SpotLightComponent& SpotLightComponent::setCutOff(float rad)
	{
		_light.cutOff = rad;
		return *this;
	}
	SpotLightComponent& SpotLightComponent::setOuterCutOff(float rad)
	{
		_light.outerCutOff = rad;
		return *this;
	}
	DirLightComponent& DirLightComponent::setSize(float size)
	{
		light.size = size;
		return *this;
	}
	DirLightComponent& DirLightComponent::setDirection(vec3 dir)
	{
		light.dir = dir.new_normalized();
		return *this;
	}
	DirLightComponent& DirLightComponent::setColor(const vec3& color, float ambientRatio)
	{
		light.ambient = color * ambientRatio;
		light.color = color - light.ambient;
		return *this;
	}
	DirLightComponent& DirLightComponent::enableShadow(bool value)
	{
		light.useShadow = value;
		return *this;
	}
	DirLightComponent& DirLightComponent::createShadowMap(uvec2 size, TextureFormat format)
	{
		ImageInfo img = { size, format };
		Sampler2DInfo sampler = { TextureFiltering::Near, TextureWrap::ClampToEdge }; //TODO: change to clamp to border
		light.shadowMap.create(Texture::Settings{ img, format, sampler, 1 });
		light.useShadow = true;
		return *this;
	}
	void DirLightComponent::update(float delta)
	{
		if (light.useShadow && prevDir != light.dir)
		{
			prevDir = light.dir;

			mat4 view;
			view.setViewMatrix(node->getTransform().getPosition(), -light.dir, vec3(0.f, 0.f, 1.f));
			mat4 proj;
			proj.setOrtho(10.f, 1.f, 0.1f, 25.f);
			light.viewProj = proj * view;
		}
	}
}
