#pragma once

#include <Age/Object/Object3D.hpp>

using namespace a_game_engine;

namespace a_game
{
	class Monkie : public Object3D
	{
	public:
		const Texture2D* voidTex = nullptr;

		Monkie();
		void update(float delta) override;
		virtual void draw(const Scene3D&, const Camera3D&, const Shader* s = nullptr) const override;
	};
}