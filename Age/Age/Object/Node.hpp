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
			Opaque, Transparent
		};

		using Container = std::forward_list<std::unique_ptr<Node>>;

		const Type type = Type::Opaque;
		Scene* scene = nullptr;
		Node* parent = nullptr;
		Container children;
		Container transparentChildren;
		std::vector<std::unique_ptr<Component>> components;

		Node(Type type = Type::Opaque);

		template <typename T>
		T* findComponent()
		{
			for (auto& comp : components)
			{
				T* castedComp = comp->as<T*>();
				if (castedComp)
					return castedComp;
			}
			return nullptr;
		}
		template <typename T>
		const T* findComponentConst() const
		{
			for (auto& comp : components)
			{
				const T* castedComp = comp->as<const T*>();
				if (castedComp)
					return castedComp;
			}
			return nullptr;
		}
		template <typename T>
		std::vector<T*> findAllComponents()
		{
			std::vector<T*> result;
			for (auto& comp : components)
			{
				T* castedComp = comp->as<T>();
				if (castedComp)
					result.push_back(castedComp);
			}
			return result;
		}
		template <typename T>
		std::vector<const T*> findAllComponentsConst() const
		{
			std::vector<const T*> result;
			for (auto& comp : components)
			{
				const T* castedComp = comp->as<const T>();
				if (castedComp)
					result.push_back(castedComp);
			}
			return result;
		}

		void addComponent(std::unique_ptr<Component>&& comp);
		template <typename T>
		T& addComponent()
		{
			auto comp = std::make_unique<T>(*this);
			T* result = comp.get();
			addComponent(std::move(comp));
			return *result;
		}

		void addChild(std::unique_ptr<Node>&& node);
		Node& addChild(Type type = Opaque);
		void forEach(std::function<void(Node&)> func);
		void forEachConst(std::function<void(const Node&)> func) const;
		void forEachLocal(std::function<void(Node&)> func);
		void forEachConstLocal(std::function<void(const Node&)> func) const;

		bool isTransparent() const {
			return bool(type & Type::Transparent);
		}

		void handleRawEvents(const sf::Event& ev);
		void handleEvents(const EventHandler& ev, float delta);
		void update(float delta);
		virtual void draw(const SceneInfo& info) const;

		virtual ~Node() = default;
	};
}