#pragma once

#include <SFML/Window.hpp>
#include "Age/types.hpp"
#include "Age/Math/vec2.hpp"
#include "Age/EventHandler.hpp"

namespace a_game_engine
{
	class Application
	{
		void initEvents();
	protected:
		sf::Window _window;
		EventHandler _eventHandler;
	public:
		void run(uvec2 windowSize, const sf::String& title, int style, uint antialiasing, uint depthBits);

		bool createWindow(uvec2 size, const sf::String& title, int style, uint antialiasing, uint depthBits);
		virtual void init() = 0;
		virtual void loadResources() = 0;
		virtual void handleRawEvents(const sf::Event& ev) = 0;
		virtual void handleEvents(float delta) = 0;
		virtual void update(float delta) = 0;
		virtual void draw(float delta) const = 0;
		virtual void onExit() = 0;
	};
}