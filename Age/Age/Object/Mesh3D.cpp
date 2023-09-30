#include "Age/Object/Mesh3D.hpp"
#include "Age/Material/Shader.hpp"

namespace a_game_engine
{
	void Mesh3D::draw(const RenderInfo& info) const
	{
		const Shader* shTemp = info.shader ? info.shader : material.shader;
		if (shTemp == nullptr)
			return;
		const ShaderProgram* s = &shTemp->getProgram(*info.shaderSettings);

		s->use();
		for (const auto& ext : *info.external)
			s->setUniform(ext);
		for (const auto& prop : material.props)
			s->setUniform(prop);
		s->setTransformProps(*info.transform);

		buffer.draw();
	}
}
