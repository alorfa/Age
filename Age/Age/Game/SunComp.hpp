#pragma once

#include <Age/Object/Component.hpp>

namespace a_game_engine
{
	class Node;
	class DirLightComponent;
}

using namespace a_game_engine;

namespace a_game
{
	class WorldScene;

	class SunComp : public Component
	{
		Node* node = nullptr;
		DirLightComponent* dirl = nullptr;
		const WorldScene* world = nullptr;
	public:
		SunComp(Node& n);

		void update(float delta) override;
	};
}
