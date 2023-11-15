#include "Node.hpp"
#include <SFML/Window/Event.hpp>
#include "Age/EventHandler.hpp"

namespace a_game_engine
{
	Transform& Node::changeTransform()
	{
		forEach([](Node& n)
			{
				n.getTransform().markParent();
			});
		return _transform;
	}
	void Node::setPosition(const vec3& pos)
	{
		if (getTransform().getPosition() != pos)
			changeTransform().changePosition() = pos;
	}
	void Node::setEulerRotation(const vec3& rot)
	{
		if (getTransform().getRotation() != rot)
			changeTransform().changeRotation() = rot;
	}
	void Node::setScale(const vec3& scale)
	{
		if (getTransform().getScale() != scale)
			changeTransform().changeScale() = scale;
	}

	Node::Node(Type type)
		: type(type) {}

	void Node::addComponent(std::unique_ptr<Component>&& comp)
	{
		components.push_back(std::move(comp));
	}
	void Node::addChild(std::unique_ptr<Node>&& node)
	{
		node->_transform._parent = &_transform;
		node->parent = this;
		node->scene = scene;
		if (node->isTransparent())
			transparentChildren.push_front(std::move(node));
		else
			children.push_front(std::move(node));
	}
	Node& Node::addChild(Type type)
	{
		auto n = std::make_unique<Node>(type);
		Node* result = n.get();
		addChild(std::move(n));
		return *result;
	}
	void Node::forEach(std::function<void(Node&)> func)
	{
		for (const auto& node : children)
		{
			func(*node);
			node->forEach(func);
		}
		for (const auto& node : transparentChildren)
		{
			func(*node);
			node->forEach(func);
		}
	}
	void Node::forEachConst(std::function<void(const Node&)> func) const
	{
		for (const auto& node : children)
		{
			func(*node);
			node->forEachConst(func);
		}
		for (const auto& node : transparentChildren)
		{
			func(*node);
			node->forEachConst(func);
		}
	}
	void Node::forEachLocal(std::function<void(Node&)> func)
	{
		for (const auto& node : children)
			func(*node);
		for (const auto& node : transparentChildren)
			func(*node);
	}
	void Node::forEachConstLocal(std::function<void(const Node&)> func) const
	{
		for (const auto& node : children)
			func(*node);
		for (const auto& node : transparentChildren)
			func(*node);
	}
	void Node::handleRawEvents(const sf::Event& ev)
	{
		for (auto& comp : components)
			comp->handleRawEvents(ev);

		forEachLocal([&](Node& n) {
			n.handleRawEvents(ev);
		});
	}
	void Node::handleEvents(const EventHandler& ev, float delta)
	{
		for (auto& comp : components)
			comp->handleEvents(ev, delta);

		forEachLocal([&](Node& n) {
			n.handleEvents(ev, delta);
		});
	}
	void Node::update(float delta)
	{
		for (auto& comp : components)
			comp->update(delta);

		forEachLocal([&](Node& n) {
			n.update(delta);
		});
	}
	void Node::draw(const SceneInfo& info) const
	{
		for (auto& comp : components)
			comp->draw(info);
	}
}