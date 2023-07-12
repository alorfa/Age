#include "EventHandler.hpp"

namespace a_game_engine
{
	void EventHandler::clearEvents()
	{
		for (auto& e : _events)
			e.result = false;
	}
	void EventHandler::handleEvent(const sf::Event& ev)
	{
		for (auto& e : _events)
			e.result |= e.handler(ev);
	}
	void EventHandler::setEvent(const std::string& eventName, std::function<bool(const sf::Event&)> func)
	{
		int index = (int)_events.size(); //TODO: replace if event already exists
		_events.push_back({ false, func });

		_names.insert(std::pair(eventName, index));
	}
	bool EventHandler::getEvent(const std::string& eventName) const
	{
		int eventIndex = _names.at(eventName);
		return _events[eventIndex].result;
	}
	void EventHandler::removeEvent(const std::string& eventName)
	{
		//TODO
	}
}