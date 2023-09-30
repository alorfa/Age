#include "Object3D.hpp"
#include "Age/Object/Model3D.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Scene/Scene3D.hpp"
#include "Age/Material/Shader.hpp"

namespace a_game_engine
{
	Object3D::Object3D(Scene3D& scene, Node3D* parent, Node3D::Type type)
		: Node3D(scene, parent, type)
	{ }

	void Object3D::setShader(const Shader& s)
	{
		if (model)
		{
			for (auto& mesh : model->meshes)
			{
				mesh->material.shader = &s;
			}
		}
	}

	void Object3D::draw(const mat4& parent, const Scene3DInfo& info) const
	{
		if (model)
		{
			Pipeline::setDepthFunc(DepthFunc::Less);

			mat4 curTransform = parent * transform.getMatrix();
			model->draw(curTransform, info);
		}

		Node3D::draw(parent, info);
	}
}
