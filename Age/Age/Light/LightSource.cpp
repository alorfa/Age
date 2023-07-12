#include "LightSource.hpp"
#include <Age/Scene/Scene3D.hpp>

namespace a_game_engine
{
	PointLightSource::PointLightSource(const Model3D& m, const Shader& sh)
	{
		model = &m;
		shader = &sh;
	}
	void PointLightSource::update(float delta)
	{
		light.pos = transform.getPosition();
	}
	void PointLightSource::draw(const Scene3D& sc, const Camera3D& camera, const Shader* s) const
	{
		if (s == nullptr)
			s = shader;

		s->use();
		s->setUniform("view", camera.transform.getMatrix());
		s->setUniform("projection", camera.getProjection());
		s->setUniform("sceneAmbient", sc.ambient);
		s->setUniform("lightColor", light.color);

		model->draw(transform.getMatrix(), *s);
	}

	SpotLightSource::SpotLightSource(const Model3D& m, const Shader& sh)
	{
		model = &m;
		shader = &sh;
	}
	void SpotLightSource::update(float delta)
	{
		light.pos = transform.getPosition();
		light.dir = Transform3D::getForwardDir(transform.getRotation());
	}
	void SpotLightSource::draw(const Scene3D& sc, const Camera3D& camera, const Shader* s) const
	{
		if (s == nullptr)
			s = shader;

		s->use();
		s->setUniform("view", camera.transform.getMatrix());
		s->setUniform("projection", camera.getProjection());
		s->setUniform("sceneAmbient", sc.ambient);
		s->setUniform("lightColor", light.color);

		model->draw(transform.getMatrix(), *s);
	}
}