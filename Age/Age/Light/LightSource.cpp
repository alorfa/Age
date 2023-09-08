#include "LightSource.hpp"
#include "Age/LL/Shader/Shader.hpp"
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	PointLightSource::PointLightSource(Scene3D& scene, Node3D* parent)
		: Object3D(scene, parent, Node3D::Influencing)
	{ }

	void PointLightSource::draw(const mat4& parent, const Camera3D& c, const Shader* s) const
	{
		if (s == nullptr)
			s = shader;

		s->use();
		s->setUniform("lightColor", light.color);

		Object3D::draw(parent, c, s);
	}

	SpotLightSource::SpotLightSource(Scene3D& scene, Node3D* parent)
		: Object3D(scene, parent, Node3D::Influencing)
	{ }

	void SpotLightSource::draw(const mat4& parent, const Camera3D& c, const Shader* s) const
	{
		if (s == nullptr)
			s = shader;

		s->use();
		s->setUniform("lightColor", light.color);

		Object3D::draw(parent, c, s);
	}

	DirLightSource::DirLightSource(Scene3D& scene, Node3D* parent)
		: Node3D(scene, parent, Node3D::Influencing)
	{}
}