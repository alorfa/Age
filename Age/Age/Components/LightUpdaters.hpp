#pragma once

#include "Age/Object/Component.hpp"
#include "Age/Light/LightSource.hpp"

namespace a_game_engine
{
	class PointLightUpdater : public Component
	{
		PointLightSource* const light;
	public:
		inline PointLightUpdater(PointLightSource& light)
			: light(&light) {}

		void update(float delta) override;
	};

	class SpotLightUpdater : public Component
	{
		SpotLightSource* const light;
	public:
		inline SpotLightUpdater(SpotLightSource& light)
			: light(&light) {}

		void update(float delta) override;
	};
}