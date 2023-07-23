#include "Node3D.hpp"
#include <SFML/Window/Event.hpp>
#include "Age/EventHandler.hpp"

namespace a_game_engine
{
	Node3D::Node3D(Node3D* parent)
		: parent(parent)
	{}
	void Node3D::addComponent(std::unique_ptr<Component>&& comp)
	{
		components.push_back(std::move(comp));
	}
	void Node3D::addChild(std::unique_ptr<Node3D>&& node)
	{
		if (node->isInfluencing)
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
		forEach([&](Node3D& n) {
			for (auto& comp : n.components)
			{
				comp->handleRawEvents(ev);
			}
			n.handleRawEvents(ev);
		});
	}
	void Node3D::handleEvents(const EventHandler& ev, float delta)
	{
		forEach([&](Node3D& n) {
			for (auto& comp : n.components)
			{
				comp->handleEvents(ev, delta);
			}
			n.handleEvents(ev, delta);
		});
	}
	void Node3D::update(float delta)
	{
		forEach([&](Node3D& n) {
			for (auto& comp : n.components)
			{
				comp->update(delta);
			}
			n.update(delta);
		});
	}
	void Node3D::draw(const mat4& parent, const Node3D& sc, const Camera3D& c, const Shader* s) const
	{
		if (s == nullptr)
			s = shader;
		mat4 curTransform = parent * transform.getMatrix();
		forEachConst([&](const Node3D& n)
			{
				n.draw(curTransform, sc, c, s);
			});
	}
}