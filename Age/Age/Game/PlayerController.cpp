#include "PlayerController.hpp"
#include <SFML/Window/Event.hpp>
#include <Age/egd.hpp>
#include <Age/Math/Math.hpp>
#include <Age/EventHandler.hpp>
#include <imgui.h>
#include "Age/Game/WorldScene.hpp"

namespace a_game
{
	PlayerController::PlayerController(const Node& n)
	{
		_scene = dynamic_cast<WorldScene*>(n.scene);
	}
	void PlayerController::handleRawEvents(const sf::Event& ev)
	{
		if (ev.type == sf::Event::Resized)
		{
			uvec2 newSize = { ev.size.width ,ev.size.height };
			egd.camera.setAspectRatio(newSize);
		}
	}
	void PlayerController::handleEvents(const EventHandler& ev, float delta)
	{
		if (ev.getEvent("camera"))
			setCameraActive(!enableControl);

		if (enableControl)
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
		else
		{
			const int flag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
			ImGui::Begin("A Game", nullptr, 0);
			if (ImGui::CollapsingHeader("Renderer"))
			{
				ImGui::RadioButton("Forward renderer", &_scene->rendererIndex, 0);
				ImGui::RadioButton("Deferred renderer", &_scene->rendererIndex, 1);
				ImGui::Checkbox("Show gbuffers", &_scene->changeDeferredRendererDebug());
			}
			if (ImGui::CollapsingHeader("Postprocessing"))
			{
				ImGui::SliderFloat("Bloom radius(in pixels)", &_scene->bloom.radius, 1.f, 2.f);
				ImGui::SliderInt("Bloom mip count", &_scene->bloomMipCount, 1, 7);
				ImGui::Checkbox("Bloom as fog", &_scene->bloomFogBlending);
				ImGui::SliderFloat("Bloom strength", &_scene->bloom.strength, 0.f, 
					_scene->bloomFogBlending ? 0.7f : 0.2f, "%.2f");
				ImGui::SliderFloat("Fog distance", &_scene->fogDistance, 0.f, 40.f);
				ImGui::ColorEdit3("Fog color", (float*)(&_scene->fogColor));
				ImGui::Checkbox("Bright sun", &_scene->isSunny);
			}
			if (ImGui::CollapsingHeader("Presets"))
			{
				if (ImGui::Button("Sun day"))
				{
					_scene->bloom.strength = 0.05f;
					_scene->bloomFogBlending = false;
					_scene->bloomMipCount = 7;
					_scene->bloom.radius = 1.3f;
					_scene->fogDistance = 0.f;
				}
				if (ImGui::Button("Fog"))
				{
					_scene->bloom.strength = 0.45f;
					_scene->bloomFogBlending = true;
					_scene->bloomMipCount = 5;
					_scene->bloom.radius = 1.2f;
					_scene->fogDistance = 25.f;
					_scene->rendererIndex = 1;
					_scene->fogColor = vec3{ 0.463f, 0.5f, 0.618f };
				}
			}
			ImGui::End();
		}
	}
	void PlayerController::setCameraActive(bool value)
	{
		if (value == enableControl)
			return;

		enableControl = value;
		if (enableControl)
		{
			auto sfPos = sf::Mouse::getPosition(*egd.window);
			stopMouse.x = sfPos.x;
			stopMouse.y = sfPos.y;
			egd.window->setMouseCursorVisible(false);
		}
		else
		{
			egd.window->setMouseCursorVisible(true);
		}
	}
}

