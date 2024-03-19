#include "FlashLight.hpp"
#include <Age/Transform/Camera.hpp>
#include <Age/Object/Node.hpp>
#include "FlashLight.hpp"
#include "Age/EventHandler.hpp"

using namespace a_game_engine;

namespace a_game
{
	FlashLight::FlashLight(Node& node, const Camera& camera)
	{
		this->node = &node;
		this->camera = &camera;
	}

	void FlashLight::handleEvents(const EventHandler& ev, float delta)
	{
		if (!spotLight)
		{
			spotLight = node->findComponent<SpotLightComponent>();
		}
		if (spotLight && ev.getEvent("mouseRight"))
		{
			if (spotLight->getLight().color.x)
			{
				spotLight->setColor({ 0.f }, 0.f);
			}
			else
			{
				spotLight->setColor({ 4.f, 4.f, 8.f }, 0.03f);
			}
		}
	}

	void FlashLight::update(float delta)
	{
		node->setPosition(camera->transform.getPosition());
		node->setEulerRotation(camera->transform.getRotation());
	}
}

