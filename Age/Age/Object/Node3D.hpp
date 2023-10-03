#pragma once

#include <forward_list>
#include <memory>
#include <functional>
#include "Age/Transform/Transform3D.hpp"
#include "Component.hpp"
#include "Age/Scene/Scene3DInfo.hpp"

namespace sf
{
	class Event;
}

namespace a_game_engine
{
	class Camera3D;
	class Shader;
	class EventHandler;
	class Scene3D;

	class Node3D
	{
	public:
		enum Type : unsigned int
		{
			Usual, Influencing
		};

		using Container = std::forward_list<std::unique_ptr<Node3D>>;

		const Type type = Type::Usual;
		Scene3D* const scene;
		Node3D* const parent;
		Container children;
		Container infChildren;
		std::vector<std::unique_ptr<Component>> components;

		Transform3D transform;

		Node3D(Scene3D& scene, Node3D* parent, Type type = Type::Usual);

		void addComponent(std::unique_ptr<Component>&& comp);
		void addChild(std::unique_ptr<Node3D>&& node);
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
		template <typename T>
		const T* as() const {
			return dynamic_cast<const T*>(this);
		}
		bool isInfluencing() const {
			return bool(type & Type::Influencing);
		}

		void handleRawEvents(const sf::Event& ev);
		void handleEvents(const EventHandler& ev, float delta);
		void update(float delta);
		virtual void draw(const mat4& parent, const Scene3DInfo& info) const;

		virtual ~Node3D() = default;
	};
}