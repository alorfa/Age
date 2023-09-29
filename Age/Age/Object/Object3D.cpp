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

	void Object3D::draw(const mat4& parent, const Scene3DInfo& info) const
	{
		const Shader* shTemplate = info.shader ? info.shader : shader;
		const ShaderProgram& s = shTemplate->getProgram(info.shaderSettings);
		if (model)
		{
			Pipeline::setDepthFunc(DepthFunc::Less);
			s.use();
			s.setCamera(*info.camera);
			for (const auto& prop : info.props)
				s.setUniform(s.getLocation(prop.name.c_str()), prop.property);

			mat4 curTransform = parent * transform.getMatrix();
			model->draw(curTransform, s);
		}

		Node3D::draw(parent, info);
	}
}
