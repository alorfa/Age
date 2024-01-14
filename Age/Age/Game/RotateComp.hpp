#pragma once

#include <Age/Object/Component.hpp>

namespace a_game_engine
{
	class Node;
}

using namespace a_game_engine;

namespace a_game
{
	class RotateComp : public Component
	{
		Node* _node = nullptr;
		vec3 _center, _size;
		float _speed = 1.f;
		float _time = 0.f;
	public:
		inline RotateComp(Node& node)
			: _node(&node) {}

		void init(const vec3& center, const vec3& size, float speed);

		void update(float delta) override;
	};
}