#pragma once

#include <Age/Object/Component.hpp>

namespace a_game_engine
{
	class Node3D;
	class Camera3D;
}

using namespace a_game_engine;

namespace a_game
{
	class Rotate : public Component
	{
		Node3D* node = nullptr;
	public:
		Rotate(Node3D& node);

		void update(float delta) override;
	};
}