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
				n.changeTransform();
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
	Node::Node(Scene& scene, Node* parent, Type type)
		: scene(&scene), parent(parent), type(type), _transform(parent ? parent->_transform : nullptr)
	{ }

	void Node::addComponent(std::unique_ptr<Component>&& comp)
	{
		components.push_back(std::move(comp));
	}
	void Node::addChild(std::unique_ptr<Node>&& node)
	{
		if (node->isInfluencing())
			infChildren.push_front(std::move(node));
		else
			children.push_front(std::move(node));
	}
	void Node::forEach(std::function<void(Node&)> func)
	{
		for (const auto& node : children)
			func(*node);
		for (const auto& node : infChildren)
			func(*node);
	}
	void Node::forEachConst(std::function<void(const Node&)> func) const
	{
		for (const auto& node : children)
			func(*node);
		for (const auto& node : infChildren)
			func(*node);
	}
	void Node::handleRawEvents(const sf::Event& ev)
	{
		for (auto& comp : components)
			comp->handleRawEvents(ev);

		forEach([&](Node& n) {
			n.handleRawEvents(ev);
		});
	}
	void Node::handleEvents(const EventHandler& ev, float delta)
	{
		for (auto& comp : components)
			comp->handleEvents(ev, delta);

		forEach([&](Node& n) {
			n.handleEvents(ev, delta);
		});
	}
	void Node::update(float delta)
	{
		for (auto& comp : components)
			comp->update(delta);

		forEach([&](Node& n) {
			n.update(delta);
		});
	}
	void Node::draw(const SceneInfo& info) const
	{
	}
}