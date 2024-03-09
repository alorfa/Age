#include "Application.hpp"
#include <Age/LL/opengl.h>
#include "egd.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_win32.h>

namespace a_game_engine
{
	Egd egd = Egd();

	void Application::initEvents()
	{
		_eventHandler.setEvent("escape", [](const sf::Event& e)
			{
				return (e.type == sf::Event::KeyPressed &&
					e.key.code == sf::Keyboard::Escape);
			});
	}
	void Application::run(uvec2 windowSize, const sf::String& title, int style, uint antialiasing, uint depthBits)
	{
		createWindow(windowSize, title, style, antialiasing, depthBits);

		sf::Clock clock;
		sf::Event ev;

		initEvents();
		init();
		loadResources();

		while (_window.isOpen())
		{
			const float delta = clock.restart().asSeconds();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			memset(&ev, 0, sizeof(ev));
			_eventHandler.clearEvents();
			if (_window.hasFocus())
				_eventHandler.handleEvent(ev);
			while (_window.pollEvent(ev))
			{
				ImGui::SFML::ProcessEvent(_window, ev);
				if (_window.hasFocus())
					_eventHandler.handleEvent(ev);
				switch (ev.type)
				{
				case sf::Event::Closed:
					_window.close();
					break;
				case sf::Event::Resized:
					glViewport(0, 0, ev.size.width, ev.size.height);
					break;
				}
				handleRawEvents(ev);
			}

			handleEvents(delta);
			update(delta);
			draw(delta); 
			_window.display();
		}
		onExit(); 
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		_window.close();
	}
	bool Application::createWindow(uvec2 size, const sf::String& title, int style, uint antialiasing, uint depthBits)
	{
		_window.close();

		sf::ContextSettings settings;
		settings.attributeFlags = sf::ContextSettings::Core;
#ifdef _DEBUG
		settings.attributeFlags |= sf::ContextSettings::Debug;
#endif
		settings.majorVersion = 4;
		settings.minorVersion = 6;
		settings.antialiasingLevel = antialiasing;
		settings.depthBits = depthBits;
		settings.stencilBits = 8;
		_window.create({ size.x, size.y }, title, style, settings);

		glewExperimental = true;
		if (glewInit() != GLEW_OK) {
			_window.close();
			return false;
		}
		ImGui::SFML::Init(_window, sf::Vector2f((float)size.x, (float)size.y), true);

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		return true;
	}
}