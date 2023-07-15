#include "TestGame.hpp"
#include <Age/LL/opengl.h>
#include <Age/LL/Buffers/VertexBuffer.hpp>
#include <Age/LL/Shader/ShaderCompiler.hpp>
#include <Age/LL/Texture/Image.hpp>
#include <Age/LL/Texture/Texture2D.hpp>
#include <iostream>
#include <Age/Object/Object3D.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Age/Auxiliary/print_math.hpp>
#include <Age/Transform/Camera3D.hpp>
#include <Age/Resource/Logger.hpp>
#include "Age/Light/Light.hpp"
#include "Age/Game/Gdata.hpp"
#include "Age/Resource/ShaderMaker.hpp"

namespace a_game
{
	void TestGame::init()
	{
		world = std::make_unique<WorldScene>();
		gdata = new Gdata();
		gdata->camera.setFov(glm::radians(45.f));
		gdata->camera.transform.changeRotation().x = glm::radians(90.f);
		gdata->camera.setNearFar({ 0.1f, 100.f });
		auto size = _window.getSize();
		gdata->camera.setWindowSize({ size.x, size.y });
		gdata->window = &_window;
		gdata->res = "res";
		gdata->user = "user";

		std::cout << std::fixed;
		std::cout.precision(2);
		_window.setFramerateLimit(60); 

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		_eventHandler.setEvent("jump", [](const sf::Event& e)
			{
				return (e.type == sf::Event::KeyPressed && 
					e.key.code == sf::Keyboard::Space);
			});
		_eventHandler.setEvent("left", [](const sf::Event& e)
			{
				return sf::Keyboard::isKeyPressed(sf::Keyboard::A);
			});
		_eventHandler.setEvent("right", [](const sf::Event& e)
			{
				return sf::Keyboard::isKeyPressed(sf::Keyboard::D);
			});
		_eventHandler.setEvent("forward", [](const sf::Event& e)
			{
				return sf::Keyboard::isKeyPressed(sf::Keyboard::W);
			});
		_eventHandler.setEvent("back", [](const sf::Event& e)
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
		world->load();
		Logger::logDebug("Resources load time: " + std::to_string(clock.restart().asSeconds()));
	}
	void TestGame::handleRawEvents(const sf::Event& ev)
	{
		if (ev.type == sf::Event::Resized)
		{
			uvec2 newSize = { ev.size.width ,ev.size.height };
			gdata->camera.setWindowSize(newSize);
		}

		world->handleRawEvents(ev);
	}
	void TestGame::handleEvents(float delta)
	{
		const auto direction = Transform3D::getForwardDir(gdata->camera.transform.getRotation());
		if (_eventHandler.getEvent("forward"))
			gdata->camera.transform.changePosition() += direction * (delta * 4);
		if (_eventHandler.getEvent("back"))
			gdata->camera.transform.changePosition() -= direction * (delta * 4);

		const auto rightDir = Transform3D::getRightDir(gdata->camera.transform.getRotation());
		if (_eventHandler.getEvent("left"))
			gdata->camera.transform.changePosition() -= rightDir * (delta * 4);
		if (_eventHandler.getEvent("right"))
			gdata->camera.transform.changePosition() += rightDir * (delta * 4);

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
			vec3 rot = gdata->camera.transform.getRotation();
			rot.x += cameraOffset.y;
			rot.y -= cameraOffset.x;
			float minrot = glm::radians(0.1f);
			float maxrot = glm::radians(179.9f);
			if (rot.x <= minrot)
				rot.x = minrot;
			if (rot.x >= maxrot)
				rot.x = maxrot;
			gdata->camera.transform.changeRotation() = rot;
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
		world->draw();
	}
	void TestGame::onExit()
	{
		delete gdata;
	}
}

int main()
{
	/*try
	{
		VertexShaderMaker::Params3D params;
		params.safeScaleNormals = true;
		auto vert = VertexShaderMaker::create(params);
		Logger::logDebug(vert);
		return 0;
	}
	catch (std::exception& e)
	{
		Logger::logError(e.what());
	}*/

	auto game = std::make_unique<a_game::TestGame>();
	game->run({889, 500}, "Alina's game engine (OpenGL 3.3 core)", sf::Style::Default, 8, 24);

	return 0;
}