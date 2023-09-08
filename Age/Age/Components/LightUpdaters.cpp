#include "LightUpdaters.hpp"
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	void PointLightUpdater::update(float delta)
	{
		light->light.pos = light->transform.getPosition();
	}

	void SpotLightUpdater::update(float delta)
	{
		light->light.pos = light->transform.getPosition();
		light->light.dir = Math::getForwardDir(light->transform.getRotation());
	}
}

