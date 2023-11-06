#pragma once

#include <Age/Object/Component.hpp>

namespace a_game_engine
{
	class Node;
	class Camera;
}

using namespace a_game_engine;

namespace a_game
{
	class Rotate : public Component
	{
		Node* node = nullptr;
	public:
		Rotate(Node& node);

		void update(float delta) override;
	};
}