#pragma once

#include <forward_list>
#include <memory>
#include "Age/Transform/Transform3D.hpp"
#include <functional>

namespace sf
{
	class Event;
}

namespace a_game_engine
{
	class Camera3D;
	class Shader;
	class EventHandler;

	class Node3D
	{
	public:
		using Container = std::forward_list<std::unique_ptr<Node3D>>;

		Node3D* const parent;
		Container children;
		Container infChildren;
		bool isInfluencing = false;

		Transform3D transform;
		const Shader* shader = nullptr;

		Node3D(Node3D* parent);

		void addChild(std::unique_ptr<Node3D> node);
		void forEach(std::function<void(Node3D&)> func);
		void forEachConst(std::function<void(const Node3D&)> func) const;

		template<typename T>
		bool is() const {
			return (bool)dynamic_cast<const T*>(this);
		}
		template <typename T>
		T* as() {
			return dynamic_cast<T*>(this);
		}

		virtual void handleRawEvents(const sf::Event& ev);
		void handleRawEventsNode(const sf::Event& ev);

		virtual void handleEvents(const EventHandler& ev, float delta);
		void handleEventsNode(const EventHandler& ev, float delta);

		virtual void update(float delta);
		void updateNode(float delta);

		virtual void draw(const mat4& parent, const Node3D& scene, const Camera3D& c, const Shader* s) const;
		void drawNode(const mat4& parent, const Node3D& scene, const Camera3D& c, const Shader* s = nullptr) const;

		virtual ~Node3D() = default;
	};
}