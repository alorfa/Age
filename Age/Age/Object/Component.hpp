#pragma once

#include "Age/Scene/SceneInfo.hpp"

namespace sf
{
	class Event;
}

namespace a_game_engine
{
	class EventHandler;

	class Component
	{
	public:
		virtual void handleRawEvents(const sf::Event& ev);
		virtual void handleEvents(const EventHandler& ev, float delta);
		virtual void update(float delta);
		virtual void draw(const SceneInfo& info) const;

		virtual ~Component() = default;
	};
}
