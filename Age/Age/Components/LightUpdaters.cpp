#include "LightUpdaters.hpp"
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	void PointLightUpdater::update(float delta)
	{
		light->light.pos = light->getTransform().getPosition();
	}

	void SpotLightUpdater::update(float delta)
	{
		light->light.pos = light->getTransform().getPosition();
		light->light.dir = Math::getForwardDir(light->getTransform().getRotation());
	}
}

