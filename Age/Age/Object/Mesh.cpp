#include "Age/Object/Mesh.hpp"
#include "Age/Material/Shader.hpp"

namespace a_game_engine
{
	void Mesh::draw(const RenderInfo& info) const
	{
		const Shader* shTemp = info.shader ? info.shader : material.shader;
		if (shTemp == nullptr or not buffer)
			return;
		const ShaderProgram* s = &shTemp->getProgram(*info.shaderSettings);

		s->use();
		for (const auto& ext : *info.external)
			s->setUniform(ext);
		for (const auto& prop : material.props)
			s->setUniform(prop);
		s->setTransformProps(*info.transform);

		buffer->draw();
	}
}
