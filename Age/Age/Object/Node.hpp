#pragma once

#include <forward_list>
#include <memory>
#include <functional>
#include "Age/Transform/Transform.hpp"
#include "Component.hpp"
#include "Age/Scene/SceneInfo.hpp"

namespace sf
{
	class Event;
}

namespace a_game_engine
{
	class Camera;
	class Shader;
	class EventHandler;
	class Scene;

	class Node
	{
		Transform _transform;
	public:
		inline const Transform& getTransform() const { return _transform; }
		Transform& changeTransform();
		void setPosition(const vec3& pos);
		void setEulerRotation(const vec3& rot);
		void setScale(const vec3& scale);

		enum Type : unsigned int
		{
			Usual, Influencing
		};

		using Container = std::forward_list<std::unique_ptr<Node>>;

		const Type type = Type::Usual;
		Scene* const scene;
		Node* const parent;
		Container children;
		Container infChildren;
		std::vector<std::unique_ptr<Component>> components;

		Node(Scene& scene, Node* parent, Type type = Type::Usual);

		void addComponent(std::unique_ptr<Component>&& comp);
		void addChild(std::unique_ptr<Node>&& node);
		void forEach(std::function<void(Node&)> func);
		void forEachConst(std::function<void(const Node&)> func) const;

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
		virtual void draw(const SceneInfo& info) const;

		virtual ~Node() = default;
	};
}