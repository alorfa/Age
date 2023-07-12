#pragma once

#include <Age/Object/Object3D.hpp>

using namespace a_game_engine;

namespace a_game
{
	class Cube : public Object3D
	{
	public:
		float time = 0.f;

		Cube();
		void update(float delta) override;
		virtual void draw(const Scene3D&, const Camera3D&, const Shader* s = nullptr) const override;
	};
}