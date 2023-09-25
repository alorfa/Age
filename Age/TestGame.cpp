#include "TestGame.hpp"
#include <Age/LL/opengl.h>
#include <Age/LL/Buffers/VertexBuffer.hpp>
#include <Age/LL/Texture/Image.hpp>
#include <Age/LL/Texture/Texture2D.hpp>
#include <iostream>
#include <Age/Object/Object3D.hpp>
#include <Age/Auxiliary/print_math.hpp>
#include <Age/Transform/Camera3D.hpp>
#include <Age/Resource/Logger.hpp>
#include "Age/Light/Light.hpp"
#include "Age/Game/Gdata.hpp"
#include "Age/egd.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Math/Math.hpp"
#include "Age/Material/Shader.hpp"
#include "Age/Resource/File.hpp"

namespace a_game
{
	namespace
	{
		void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, 
			GLsizei length, GLchar const* message, void const* user_param)
		{
			const auto src_str = [source]() {
				switch (source)
				{
				case GL_DEBUG_SOURCE_API: return "API";
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
				case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
				case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
				case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
				default: return "OTHER";
				}
			}();

			auto const type_str = [type]() {
				switch (type)
				{
				case GL_DEBUG_TYPE_ERROR: return "ERROR";
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
				case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
				case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
				case GL_DEBUG_TYPE_MARKER: return "MARKER";
				default: return "OTHER";
				}
			}();

			auto const severity_str = [severity]() {
				switch (severity) {
				case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
				case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
				case GL_DEBUG_SEVERITY_LOW: return "LOW";
				default: return "NOTIFICATION";
				}
			}();
			Logger::logError(std::format("{}, {}, {}, {}: {}", src_str, type_str, severity_str, id, message));
		}
	}

	void TestGame::init()
	{
		gdata = new Gdata();
		egd.camera.setFov(Math::rad(45.f));
		egd.camera.transform.changeRotation().x = Math::rad(90.f);
		egd.camera.setNearFar({ 0.1f, 100.f });
		auto size = _window.getSize();
		egd.camera.setAspectRatio({ size.x, size.y });
		egd.window = &_window;

		std::cout << std::fixed;
		std::cout.precision(2);
		_window.setFramerateLimit(60);

		Pipeline::setUnpackAlignment(1);
		
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(message_callback, nullptr);

		_eventHandler.setEvent("jump", [](const sf::Event& e)
			{
				return (e.type == sf::Event::KeyPressed && 
					e.key.code == sf::Keyboard::Space);
			});
		_eventHandler.setEvent("left", [](const sf::Event& e)
			{
				return sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
			});
		_eventHandler.setEvent("right", [](const sf::Event& e)
			{
				return sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
			});
		_eventHandler.setEvent("a", [](const sf::Event& e)
			{
				return sf::Keyboard::isKeyPressed(sf::Keyboard::A);
			});
		_eventHandler.setEvent("d", [](const sf::Event& e)
			{
				return sf::Keyboard::isKeyPressed(sf::Keyboard::D);
			});
		_eventHandler.setEvent("w", [](const sf::Event& e)
			{
				return sf::Keyboard::isKeyPressed(sf::Keyboard::W);
			});
		_eventHandler.setEvent("s", [](const sf::Event& e)
			{
				return sf::Keyboard::isKeyPressed(sf::Keyboard::S);
			});
		_eventHandler.setEvent("mouseLeft", [](const sf::Event& e)
			{
				return e.type == sf::Event::MouseButtonPressed &&
					e.mouseButton.button == sf::Mouse::Left;
			});
		_eventHandler.setEvent("mouseLeftReleased", [](const sf::Event& e)
			{
				return e.type == sf::Event::MouseButtonReleased &&
					e.mouseButton.button == sf::Mouse::Left;
			});
	}
	void TestGame::loadResources()
	{
		sf::Clock clock;
		world = std::make_unique<WorldScene>();
		world->load();
		Logger::logDebug("Resources load time: " + std::to_string(clock.restart().asSeconds()));
	}
	void TestGame::handleRawEvents(const sf::Event& ev)
	{
		if (ev.type == sf::Event::Resized)
		{
			uvec2 newSize = { ev.size.width ,ev.size.height };
			egd.camera.setAspectRatio(newSize);
		}

		world->handleRawEvents(ev);
	}
	void TestGame::handleEvents(float delta)
	{
		const auto direction = Math::getForwardDir(egd.camera.transform.getRotation());
		if (_eventHandler.getEvent("w"))
			egd.camera.transform.changePosition() += direction * (delta * 4);
		if (_eventHandler.getEvent("s"))
			egd.camera.transform.changePosition() -= direction * (delta * 4);

		const auto rightDir = Math::getRightDir(egd.camera.transform.getRotation());
		if (_eventHandler.getEvent("a"))
			egd.camera.transform.changePosition() -= rightDir * (delta * 4);
		if (_eventHandler.getEvent("d"))
			egd.camera.transform.changePosition() += rightDir * (delta * 4);

		if (_eventHandler.getEvent("left"))
			egd.camera.transform.changeRotation().z += delta = 0.05f;
		if (_eventHandler.getEvent("right"))
			egd.camera.transform.changeRotation().z -= delta = 0.05f;

		const auto up = Math::getUpDir(egd.camera.transform.getRotation());

		if (_eventHandler.getEvent("mouseLeft"))
		{
			auto sfPos = sf::Mouse::getPosition(_window);
			stopMouse.x = sfPos.x;
			stopMouse.y = sfPos.y;
			_window.setMouseCursorVisible(false);
			mouseIsCamera = true;
		}
		if (_eventHandler.getEvent("mouseLeftReleased"))
		{
			_window.setMouseCursorVisible(true);
			mouseIsCamera = false;
		}

		if (mouseIsCamera)
		{
			auto windowSize = _window.getSize();
			auto curPos = sf::Mouse::getPosition(_window);
			ivec2 mouseOffset = { curPos.x - stopMouse.x, stopMouse.y - curPos.y };
			sf::Mouse::setPosition(sf::Vector2i(stopMouse.x, stopMouse.y), _window);
			vec2 cameraOffset{
				(float)mouseOffset.x / (float)windowSize.y * sensitivity,
				(float)mouseOffset.y / (float)windowSize.y * sensitivity
			};
			vec3 rot = egd.camera.transform.getRotation();
			rot.x += cameraOffset.y;
			rot.y -= cameraOffset.x;
			float minrot = glm::radians(0.1f);
			float maxrot = glm::radians(179.9f);
			if (rot.x <= minrot)
				rot.x = minrot;
			if (rot.x >= maxrot)
				rot.x = maxrot;
			egd.camera.transform.changeRotation() = rot;
		}
		world->handleEvents(_eventHandler, delta);
	}
	void TestGame::update(float delta)
	{
		time += delta;
		world->update(delta);
	}
	void TestGame::draw() const
	{
		world->draw(*world->activeCamera, nullptr);
	}
	void TestGame::onExit()
	{
		delete gdata;
	}
}

int main()
{
	egd.res = "res";
	egd.user = "user";
	ShaderSettings::paintingFunctions.push_back(
		"age_FragColor.rgb = age_base_color.rgb;\n"
		"age_FragColor.a = age_roughness;\n"
		"age_FragColor1.rgb = age_normal.rgb;\n"
		"age_FragColor1.a = age_metalness;\n"
		"age_FragColor2.rgb = fragPos;\n");
	ShaderSettings::include.vertInc = File::readAllText(egd.res / "shader/lib/vertInc.hasl");
	ShaderSettings::include.fragInc = File::readAllText(egd.res / "shader/lib/fragInc.hasl");
	ShaderSettings::rawInclude.vertMain =
		ShaderSettings::include.vertMain = File::readAllText(egd.res / "shader/lib/vertMain.hasl");
	ShaderSettings::rawInclude.fragMain = 
		ShaderSettings::include.fragMain = File::readAllText(egd.res / "shader/lib/fragMain.hasl");
	ShaderSettings::postprocVsh = File::readAllText(egd.res / "shader/lib/postproc.vsh");
	
	auto game = std::make_unique<a_game::TestGame>();
	game->run({889, 500}, "Alina's game engine (OpenGL 3.3 core)", sf::Style::Default, 0, 24);

	return 0;
}