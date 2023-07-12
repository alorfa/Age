#pragma once

#include <SFML/Window/Event.hpp>
#include <map>
#include <functional>

namespace a_game_engine
{
	struct EventInfo
	{
		bool result;
		std::function<bool(const sf::Event& ev)> handler;
	};

	class EventHandler
	{
		std::vector<EventInfo> _events;
		std::map<std::string, int> _names;
	public:
		void clearEvents();
		void handleEvent(const sf::Event& e);

		void setEvent(const std::string& eventName, std::function<bool(const sf::Event&)> func);
		bool getEvent(const std::string& eventName) const;
		void removeEvent(const std::string& eventName);
	};
}