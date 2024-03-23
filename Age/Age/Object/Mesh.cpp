#include "Age/Object/Mesh.hpp"
#include "Age/Material/Shader.hpp"
#include "Age/Material/ShaderSettings.hpp"

namespace a_game_engine
{
	void Mesh::draw(const RenderInfo& info) const
	{
		const Shader* shTemp = info.shader ? info.shader : material.shader;
		if (shTemp == nullptr or not buffer)
			return;
		const ShaderProgram* s = &shTemp->getProgram(*info.shaderSettings);

		bool needToUseProps = true;
		if (info.shaderSettings->type == ShaderSettings::Common::Type::Deferred)
		{
			auto& def = std::get<ShaderSettings::Deferred>(info.shaderSettings->settings);
			if (ShaderSettings::deferredImpls[def.implIndex].bindings.size() == 0)
				needToUseProps = false;
		}

		s->use();
		for (const auto& ext : *info.external)
			s->setUniform(ext);
		if (needToUseProps)
			for (const auto& prop : material.props)
				s->setUniform(prop);
		s->setTransformProps(*info.transform);

		buffer->draw();
	}
}
