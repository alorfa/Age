#include "ControlController.hpp"
#include <SFML/Window/Event.hpp>
#include <Age/egd.hpp>
#include <Age/Math/Math.hpp>
#include <Age/EventHandler.hpp>

namespace a_game
{
	void ControlController::handleRawEvents(const sf::Event& ev)
	{
		if (ev.type == sf::Event::Resized)
		{
			uvec2 newSize = { ev.size.width ,ev.size.height };
			egd.camera.setAspectRatio(newSize);
		}
	}
	void ControlController::handleEvents(const EventHandler& ev, float delta)
	{
		const auto direction = Math::getForwardDir(egd.camera.transform.getRotation());
		if (ev.getEvent("w"))
			egd.camera.transform.changePosition() += direction * (delta * 4);
		if (ev.getEvent("s"))
			egd.camera.transform.changePosition() -= direction * (delta * 4);

		const auto rightDir = Math::getRightDir(egd.camera.transform.getRotation());
		if (ev.getEvent("a"))
			egd.camera.transform.changePosition() -= rightDir * (delta * 4);
		if (ev.getEvent("d"))
			egd.camera.transform.changePosition() += rightDir * (delta * 4);

		const auto up = Math::getUpDir(egd.camera.transform.getRotation());

		if (ev.getEvent("mouseLeft"))
		{
			auto sfPos = sf::Mouse::getPosition(*egd.window);
			stopMouse.x = sfPos.x;
			stopMouse.y = sfPos.y;
			egd.window->setMouseCursorVisible(false);
			mouseIsCamera = true;
		}
		if (ev.getEvent("mouseLeftReleased"))
		{
			egd.window->setMouseCursorVisible(true);
			mouseIsCamera = false;
		}

		if (mouseIsCamera)
		{
			auto windowSize = egd.window->getSize();
			auto curPos = sf::Mouse::getPosition(*egd.window);
			ivec2 mouseOffset = { curPos.x - stopMouse.x, stopMouse.y - curPos.y };
			sf::Mouse::setPosition(sf::Vector2i(stopMouse.x, stopMouse.y), *egd.window);
			vec2 cameraOffset{
				(float)mouseOffset.x / (float)windowSize.y * sensitivity,
				(float)mouseOffset.y / (float)windowSize.y * sensitivity
			};
			vec3 rot = egd.camera.transform.getRotation();
			rot.x += cameraOffset.y;
			rot.y -= cameraOffset.x;
			const float minrot = Math::rad(0.01f);
			const float maxrot = Math::rad(179.99f);
			if (rot.x <= minrot)
				rot.x = minrot;
			if (rot.x >= maxrot)
				rot.x = maxrot;
			egd.camera.transform.changeRotation() = rot;
		}
	}
}

