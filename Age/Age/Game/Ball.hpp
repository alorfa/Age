#pragma once

#include <Age/Object/Component.hpp>

using namespace a_game_engine;

namespace a_game
{
	class Ball : public Component
	{
		Node* node = nullptr;
		vec3 velocity = { 0.f };
	public:
		Ball(Node& node);

		Ball& setVelocity(const vec3& v);

		void handleEvents(const EventHandler& ev, float delta) override;
		void update(float delta) override;
	};
}