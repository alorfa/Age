#pragma once

#include <memory>
#include "Age/Object/Node3D.hpp"
#include "SkyBox.hpp"

namespace sf
{
	class Event;
}

namespace a_game_engine
{
	class EventHandler;
	class Shader;
	class Camera3D;

	class Scene3D
	{
	public:
		const Shader* shader = nullptr;

		std::unique_ptr<Node3D> rootNode;
		SkyBox skyBox;

		Scene3D();

		virtual void load() = 0;
		virtual void handleRawEvents(const sf::Event& ev);
		virtual void handleEvents(const EventHandler& ev, float delta);
		virtual void update(float delta);
		virtual void draw(const Camera3D& c, const Shader* s) const = 0;
	};
}