#include "Node3D.hpp"
#include <SFML/Window/Event.hpp>
#include "Age/EventHandler.hpp"

namespace a_game_engine
{
	Transform3D& Node3D::changeTransform()
	{
		forEach([](Node3D& n)
			{
				n.getTransform().markParent();
				n.changeTransform();
			});
		return _transform;
	}
	void Node3D::setPosition(const vec3& pos)
	{
		if (getTransform().getPosition() != pos)
			changeTransform().changePosition() = pos;
	}
	void Node3D::setEulerRotation(const vec3& rot)
	{
		if (getTransform().getRotation() != rot)
			changeTransform().changeRotation() = rot;
	}
	void Node3D::setScale(const vec3& scale)
	{
		if (getTransform().getScale() != scale)
			changeTransform().changeScale() = scale;
	}
	Node3D::Node3D(Scene3D& scene, Node3D* parent, Type type)
		: scene(&scene), parent(parent), type(type), _transform(parent ? parent->_transform : nullptr)
	{ }

	void Node3D::addComponent(std::unique_ptr<Component>&& comp)
	{
		components.push_back(std::move(comp));
	}
	void Node3D::addChild(std::unique_ptr<Node3D>&& node)
	{
		if (node->isInfluencing())
			infChildren.push_front(std::move(node));
		else
			children.push_front(std::move(node));
	}
	void Node3D::forEach(std::function<void(Node3D&)> func)
	{
		for (const auto& node : children)
			func(*node);
		for (const auto& node : infChildren)
			func(*node);
	}
	void Node3D::forEachConst(std::function<void(const Node3D&)> func) const
	{
		for (const auto& node : children)
			func(*node);
		for (const auto& node : infChildren)
			func(*node);
	}
	void Node3D::handleRawEvents(const sf::Event& ev)
	{
		for (auto& comp : components)
			comp->handleRawEvents(ev);

		forEach([&](Node3D& n) {
			n.handleRawEvents(ev);
		});
	}
	void Node3D::handleEvents(const EventHandler& ev, float delta)
	{
		for (auto& comp : components)
			comp->handleEvents(ev, delta);

		forEach([&](Node3D& n) {
			n.handleEvents(ev, delta);
		});
	}
	void Node3D::update(float delta)
	{
		for (auto& comp : components)
			comp->update(delta);

		forEach([&](Node3D& n) {
			n.update(delta);
		});
	}
	void Node3D::draw(const Scene3DInfo& info) const
	{
	}
}