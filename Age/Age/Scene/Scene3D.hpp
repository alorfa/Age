#pragma once

#include <SFML/Window/Event.hpp>
#include <Age/Light/LightSource.hpp>
#include <forward_list>
#include "Age/Object/Object3D.hpp"
#include "Age/Light/Light.hpp"
#include "Age/Transform/Camera3D.hpp"
#include "Age/EventHandler.hpp"

namespace a_game_engine
{
	class Scene3D
	{
	public:
		std::forward_list<std::unique_ptr<Object3D>> objects;
		std::forward_list<std::unique_ptr<PointLightSource>> pointLights;
		std::forward_list<std::unique_ptr<SpotLightSource>> spotLights;
		std::forward_list<DirLight> dirLights;
		std::forward_list<std::unique_ptr<Camera3D>> cameras;
		Camera3D* activeCamera = nullptr;
		vec3 ambient;

		virtual void load() = 0;
		virtual void handleRawEvents(const sf::Event& ev);
		virtual void handleEvents(const EventHandler& ev, float delta);
		virtual void update(float delta);
		virtual void draw() const;
	};
}
