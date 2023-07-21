#pragma once

#include <Age/Object/Object3D.hpp>

using namespace a_game_engine;

namespace a_game
{
	class Cube : public Object3D
	{
	public:
		float time = 0.f;

		Cube(Node3D* parent);
		void update(float delta) override;
	};
}