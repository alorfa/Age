#include "LightSource.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/Math/Math.hpp"
#include "Age/Components/LightUpdaters.hpp"
#include "Age/Material/Shader.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Object/Model3D.hpp"

namespace a_game_engine
{
	PointLightSource::PointLightSource(Scene3D& scene, Node3D* parent)
		: Object3D(scene, parent, Node3D::Influencing)
	{
		components.push_back(std::make_unique<PointLightUpdater>(*this));
	}

	void PointLightSource::draw(const mat4& parent, const Scene3DInfo& info) const
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
			s.setUniform(s.getLocation("emission"), light.color);

			mat4 curTransform = parent * transform.getMatrix();
			model->draw(curTransform, s);
		}

		Node3D::draw(parent, info);
	}

	SpotLightSource::SpotLightSource(Scene3D& scene, Node3D* parent)
		: Object3D(scene, parent, Node3D::Influencing)
	{
		components.push_back(std::make_unique<SpotLightUpdater>(*this));
	}

	void SpotLightSource::draw(const mat4& parent, const Scene3DInfo& info) const
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
			s.setUniform(s.getLocation("emission"), light.color);

			mat4 curTransform = parent * transform.getMatrix();
			model->draw(curTransform, s);
		}

		Node3D::draw(parent, info);
	}

	DirLightSource::DirLightSource(Scene3D& scene, Node3D* parent)
		: Node3D(scene, parent, Node3D::Influencing)
	{}
}