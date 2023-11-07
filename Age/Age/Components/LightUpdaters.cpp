#include "LightUpdaters.hpp"
#include "Age/Math/Math.hpp"
#include "Age/Object/Model3D.hpp"

namespace a_game_engine
{
	void PointLightUpdater::update(float delta)
	{
		light->light.pos = light->getTransform().getPosition();
		for (auto& mesh : light->_model->meshes)
			mesh->material.setValue("emission", ShaderProperty(light->light.color));
	}

	void SpotLightUpdater::update(float delta)
	{
		light->light.pos = light->getTransform().getPosition();
	}
}

