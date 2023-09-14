#include "Object3D.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/Object/Model3D.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Scene/Scene3D.hpp"

namespace a_game_engine
{
	Object3D::Object3D(Scene3D& scene, Node3D* parent, Node3D::Type type)
		: Node3D(scene, parent, type)
	{ }

	void Object3D::draw(const mat4& parent, const Camera3D& camera, const ShaderProgram* s) const
	{
		if (s == nullptr)
			s = shader;

		if (model)
		{
			Pipeline::setDepthFunc(DepthFunc::Less);
			s->use();
			s->setCamera(camera);
			s->setLights(*scene->rootNode);
			s->setUniform("skybox", SkyBox::getSlot());

			mat4 curTransform = parent * transform.getMatrix();
			model->draw(curTransform, *s);
		}

		Node3D::draw(parent, camera, s);
	}
}
