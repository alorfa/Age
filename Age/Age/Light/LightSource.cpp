#include "LightSource.hpp"
#include "Age/LL/Shader/Shader.hpp"

namespace a_game_engine
{
	PointLightSource::PointLightSource(Node3D* parent)
		: Object3D(parent)
	{
		isInfluencing = true;
	}
	void PointLightSource::update(float delta)
	{
		light.pos = transform.getPosition();
		Object3D::update(delta);
	}

	void PointLightSource::draw(const mat4& parent, const Node3D& scene, const Camera3D& c, const Shader* s) const
	{
		if (s == nullptr)
			s = shader;

		s->use();
		s->setUniform("lightColor", light.color);

		Object3D::draw(parent, scene, c, s);
	}

	SpotLightSource::SpotLightSource(Node3D* parent)
		: Object3D(parent)
	{
		isInfluencing = true;
	}

	void SpotLightSource::update(float delta)
	{
		light.pos = transform.getPosition();
		light.dir = Transform3D::getForwardDir(transform.getRotation());
		Object3D::update(delta);
	}

	void SpotLightSource::draw(const mat4& parent, const Node3D& scene, const Camera3D& c, const Shader* s) const
	{
		if (s == nullptr)
			s = shader;

		s->use();
		s->setUniform("lightColor", light.color);

		Object3D::draw(parent, scene, c, s);
	}

	DirLightSource::DirLightSource(Node3D* parent)
		: Node3D(parent)
	{
		isInfluencing = true;
	}
}