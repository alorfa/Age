#include "LightSource.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/Math/Math.hpp"
#include "Age/Components/LightUpdaters.hpp"

namespace a_game_engine
{
	PointLightSource::PointLightSource(Scene3D& scene, Node3D* parent)
		: Object3D(scene, parent, Node3D::Influencing)
	{
		components.push_back(std::make_unique<PointLightUpdater>(*this));
	}

	void PointLightSource::draw(const mat4& parent, const Camera3D& c, const ShaderProgram* s) const
	{
		if (s == nullptr)
			s = shader;

		s->use();
		s->setUniform(s->getLocation("emission"), light.color);

		Object3D::draw(parent, c, s);
	}

	SpotLightSource::SpotLightSource(Scene3D& scene, Node3D* parent)
		: Object3D(scene, parent, Node3D::Influencing)
	{
		components.push_back(std::make_unique<SpotLightUpdater>(*this));
	}

	void SpotLightSource::draw(const mat4& parent, const Camera3D& c, const ShaderProgram* s) const
	{
		if (s == nullptr)
			s = shader;

		s->use();
		s->setUniform(s->getLocation("emission"), light.color);

		Object3D::draw(parent, c, s);
	}

	DirLightSource::DirLightSource(Scene3D& scene, Node3D* parent)
		: Node3D(scene, parent, Node3D::Influencing)
	{}
}