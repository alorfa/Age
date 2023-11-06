#pragma once

#include <memory>
#include "Age/Object/Node.hpp"
#include "SkyBox.hpp"

namespace sf
{
	class Event;
}

namespace a_game_engine
{
	class EventHandler;
	class ShaderProgram;
	class Camera;

	class Scene
	{
	public:
		const ShaderProgram* shader = nullptr;

		std::unique_ptr<Node> rootNode;
		SkyBox skyBox;

		Scene();

		virtual void load() = 0;
		virtual void handleRawEvents(const sf::Event& ev);
		virtual void handleEvents(const EventHandler& ev, float delta);
		virtual void update(float delta);
		virtual void draw(const Camera& c, const ShaderProgram* s) const = 0;
	};
}