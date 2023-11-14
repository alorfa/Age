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
		template<typename T>
		bool is() const {
			return (bool)dynamic_cast<const T*>(this);
		}
		template <typename T>
		T* as() {
			return dynamic_cast<T*>(this);
		}
		template <typename T>
		const T* as() const {
			return dynamic_cast<const T*>(this);
		}

		virtual void handleRawEvents(const sf::Event& ev);
		virtual void handleEvents(const EventHandler& ev, float delta);
		virtual void update(float delta);
		virtual void draw(const SceneInfo& info) const;

		virtual ~Component() = default;
	};
}
