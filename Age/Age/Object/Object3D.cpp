#include "Object3D.hpp"
#include "Age/LL/Shader/Shader.hpp"
#include "Age/Object/Model3D.hpp"

namespace a_game_engine
{
	Object3D::Object3D(Node3D* parent)
		: Node3D(parent)
	{
	}
	void Object3D::draw(const mat4& parent, const Node3D& sc, const Camera3D& camera,
		const Shader* s) const
	{
		if (s == nullptr)
			s = shader;

		if (model)
		{
			s->use();
			s->setCamera(camera);
			s->setLights(sc);

			mat4 curTransform = parent * transform.getMatrix();
			model->draw(curTransform, *s);
		}

		Node3D::draw(parent, sc, camera, s);
	}
}
