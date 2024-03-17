#include "SunComp.hpp"
#include <Age/Object/Node.hpp>
#include <Age/Components/LightComponents.hpp>
#include <Age/Game/WorldScene.hpp>

using namespace a_game_engine;

namespace a_game
{
	SunComp::SunComp(Node& n)
		: node(&n)
	{
		dirl = node->findComponent<DirLightComponent>();
		world = dynamic_cast<WorldScene*>(n.scene);
	}

	void SunComp::update(float delta)
	{
		vec3 baseColor = vec3{ 20.f, 20.f, 15.f };
		dirl->setColor(world->isSunny ? baseColor * 0.2f : baseColor * 0.1f, 0.f);
	}
}

