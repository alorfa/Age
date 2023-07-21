#include "Node3D.hpp"
#include <SFML/Window/Event.hpp>
#include "Age/EventHandler.hpp"

namespace a_game_engine
{
	Node3D::Node3D(Node3D* parent)
		: parent(parent)
	{}
	void Node3D::addChild(std::unique_ptr<Node3D> node)
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
	}
	void Node3D::handleRawEventsNode(const sf::Event& ev)
	{
		handleRawEvents(ev);
		forEach([&](Node3D& n) {
				n.handleRawEventsNode(ev);
			});
	}
	void Node3D::handleEvents(const EventHandler& ev, float delta)
	{
	}
	void Node3D::handleEventsNode(const EventHandler& ev, float delta)
	{
		handleEvents(ev, delta);
		forEach([&](Node3D& n) 
			{
				n.handleEventsNode(ev, delta);
			});
	}
	void Node3D::update(float delta)
	{

	}
	void Node3D::updateNode(float delta)
	{
		update(delta);
		forEach([&](Node3D& n)
			{
				n.updateNode(delta);
			});
	}
	void Node3D::draw(const mat4& t, const Node3D& sc, const Camera3D& c, const Shader* s) const
	{
	}
	void Node3D::drawNode(const mat4& t, const Node3D& sc, const Camera3D& c, const Shader* s) const
	{
		draw(t, sc, c, s);

		if (s == nullptr)
			s = shader;
		mat4 curTransform = t * transform.getMatrix();
		forEachConst([&](const Node3D& n)
			{
				n.drawNode(curTransform, sc, c, s);
			});
	}
}