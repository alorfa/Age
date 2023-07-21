#pragma once

#include <Age/Object/Object3D.hpp>

using namespace a_game_engine;

namespace a_game
{
	class Monkie : public Object3D
	{
	public:
		Monkie(Node3D* parent);
		void update(float delta) override;
	};
}