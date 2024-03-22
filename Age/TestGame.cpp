#include "TestGame.hpp"
#include <Age/LL/opengl.h>
#include <Age/LL/Buffers/VertexBuffer.hpp>
#include <Age/LL/Texture/Texture2D.hpp>
#include <iostream>
#include <Age/Object/Node.hpp>
#include <Age/Other/print_math.hpp>
#include <Age/Transform/Camera.hpp>
#include <Age/Other/Logger.hpp>
#include "Age/Light/Light.hpp"
#include "Age/Game/Gdata.hpp"
#include "Age/egd.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Math/Math.hpp"
#include "Age/Material/Shader.hpp"
#include "Age/Resource/File.hpp"
#include "Age/Material/ShaderProperty.hpp"

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
		egd.camera.transform.changePosition().z = 1.5f;
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
		_eventHandler.setEvent("mouseRight", [](const sf::Event& e)
			{
				return e.type == sf::Event::MouseButtonPressed &&
					e.mouseButton.button == sf::Mouse::Right;
			});
		_eventHandler.setEvent("mouseRightReleased", [](const sf::Event& e)
			{
				return e.type == sf::Event::MouseButtonReleased &&
					e.mouseButton.button == sf::Mouse::Right;
			});
		_eventHandler.setEvent("camera", [](const sf::Event& e)
			{
				return e.type == sf::Event::KeyPressed &&
					e.key.code == sf::Keyboard::C;
			});
	}
	void TestGame::loadResources()
	{
		ShaderProperty prop;
		sf::Clock clock;
		world = std::make_unique<WorldScene>();
		world->load();
		Logger::logDebug("Resources load time: " + std::to_string(clock.restart().asSeconds()));
	}
	void TestGame::handleRawEvents(const sf::Event& ev)
	{
		world->handleRawEvents(ev);
	}
	void TestGame::handleEvents(float delta)
	{
		world->handleEvents(_eventHandler, delta);
	}
	void TestGame::update(float delta)
	{
		world->update(delta);
	}
	void TestGame::draw(float delta) const
	{
		world->draw(nullptr, delta);
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
	ShaderSettings::deferredImpls.push_back({
		{4, 4},
		"age_FragColor.rgb = material.base_color;\n"
		"age_FragColor.a = material.roughness;\n"
		"age_FragColor1.rgb = material.normal;\n"
		"age_FragColor1.a = material.metallic;\n"
		});
	ShaderSettings::deferredImpls.push_back({
		{},
		"\n"
		});
	ShaderSettings::additionalDefines.push_back("#define AGE_DISABLE_SPECULAR\n");
	ShaderSettings::additionalDefines.push_back(
		"#define AGE_DISABLE_AMBIENT\n"
		"#define AGE_DISABLE_DIFFUSE\n");

	ShaderSettings::postprocVsh = File::readAllText(egd.res / "shader/lib/postproc.vsh");

	ShaderSettings::include.vertLib = File::readAllText(egd.res / "shader/lib/vertLib.hasl");
	ShaderSettings::include.vertInc = File::readAllText(egd.res / "shader/lib/vertInc.hasl");
	ShaderSettings::include.vertMain = File::readAllText(egd.res / "shader/lib/vertMain.hasl");

	ShaderSettings::include.fragLib = File::readAllText(egd.res / "shader/lib/fragLib.hasl");
	ShaderSettings::include.fragInc = File::readAllText(egd.res / "shader/lib/fragInc.hasl");
	ShaderSettings::include.fragMain = File::readAllText(egd.res / "shader/lib/fragMain.hasl");
	ShaderSettings::rawInclude.fragMain = File::readAllText(egd.res / "shader/lib/rawFragMain.hasl");

	ShaderSettings::rawInclude.vertLib = ShaderSettings::include.vertLib;
	ShaderSettings::rawInclude.vertMain = ShaderSettings::include.vertMain;
	ShaderSettings::rawInclude.fragLib = ShaderSettings::include.fragLib;
	
	auto game = std::make_unique<a_game::TestGame>();
	game->run({889, 500}, "A graphics engine (OpenGL 4.6 core)", sf::Style::Default, 0, 24);

	return 0;
}