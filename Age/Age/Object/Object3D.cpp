#include "Object3D.hpp"
#include "Age/Scene/Scene3D.hpp"

namespace a_game_engine
{
	void Object3D::draw(const Scene3D& sc, const Camera3D& camera, const Shader* sh, 
		std::function<void(const Object3D&, const Scene3D&, const Camera3D&, const Shader&)> func) const
	{
		if (sh == nullptr)
			sh = shader;
		func(*this, sc, camera, *sh);
	}
	void Object3D::update(float delta)
	{
	}
	void Object3D::draw(const Scene3D& sc, const Camera3D& camera, const Shader* s) const
	{
		if (model == nullptr)
			return;

		if (s == nullptr)
			s = shader;

		s->use();
		s->setCamera(camera);
		s->setLights(sc);

		model->draw(transform.getMatrix(), *s);
	}
}
