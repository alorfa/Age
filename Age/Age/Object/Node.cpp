#include "Node.hpp"
#include <SFML/Window/Event.hpp>
#include "Age/EventHandler.hpp"
#include "Age/Other/Logger.hpp"
#include <format>

namespace a_game_engine
{
	namespace
	{
		size_t getEmptyNodesCount(const Node::Container& container)
		{
			size_t result = 0;
			for (auto it = container.crbegin(); it != container.crend(); ++it)
			{
				if (it->get() == nullptr)
					result++;
				else break;
			}
			return result;
		}
	}

	Transform& Node::changeTransform()
	{
		forEach([](Node& n) {
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

	void Node::sortChildren(const vec3& point, uint nodeType, SortMode mode)
	{
		auto opProj = [&](const std::unique_ptr<Node>& node) -> float {
			if (node.get() == nullptr)
				return FLT_MAX;

			return (node->getTransform().getPosition() - point).square_length();
			};
		auto trProj = [&](const std::unique_ptr<Node>& node) -> float {
			if (node.get() == nullptr)
				return -1.f;

			return (node->getTransform().getPosition() - point).square_length();
			};
		auto opFunc = [&](float left, float right)
			{
				if (mode == SortMode::Near or mode == SortMode::Default)
					return left < right;
				return left > right;
			};
		auto trFunc = [&](float left, float right)
			{
				if (mode == SortMode::Near)
					return left < right;
				return left > right;
			};

		if ((int)nodeType & (int)Type::Opaque)
		{
			std::ranges::sort(children, opFunc, opProj);
			auto unwanted = getEmptyNodesCount(children);
			if (unwanted > 0)
				children.resize(children.size() - unwanted);
		}

		if ((int)nodeType & (int)Type::Transparent)
		{
			std::ranges::sort(transparentChildren, trFunc, trProj);
			auto unwanted = getEmptyNodesCount(transparentChildren);
			if (unwanted > 0)
				transparentChildren.resize(transparentChildren.size() - unwanted);
		}
	}
	void Node::sortBranch(const vec3& point, uint nodeType, SortMode mode)
	{
		forEach([&](Node& n) {
				sortChildren(point, nodeType, mode);
			});
	}
	void Node::destroy()
	{
		if (not parent)
			return;

		Container& container = (type == Type::Opaque) ? parent->children : parent->transparentChildren;
		auto it = std::find_if(container.begin(), container.end(), [&](const std::unique_ptr<Node>& n)
			{
				return n.get() == this;
			});
		if (it != container.end())
			it->reset();
	}
	void Node::addChild(std::unique_ptr<Node>&& node)
	{
		node->_transform._parent = &_transform;
		node->parent = this;
		node->scene = scene;
		if (node->isTransparent())
			transparentChildren.push_back(std::move(node));
		else
			children.push_back(std::move(node));
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
		func(*this);
		for (const auto& node : children)
			if (node)
				node->forEach(func);

		for (const auto& node : transparentChildren)
			if (node)
				node->forEach(func);
	}
	void Node::forEachConst(std::function<void(const Node&)> func) const
	{
		func(*this);
		for (const auto& node : children)
			if (node)
				node->forEachConst(func);

		for (const auto& node : transparentChildren)
			if (node)
				node->forEachConst(func);
	}
	void Node::forEachLocal(std::function<void(Node&)> func)
	{
		for (const auto& node : children)
			if (node)
				func(*node);
		for (const auto& node : transparentChildren)
			if (node)
				func(*node);
	}
	void Node::forEachConstLocal(std::function<void(const Node&)> func) const
	{
		for (const auto& node : children)
			if (node)
				func(*node);
		for (const auto& node : transparentChildren)
			if (node)
				func(*node);
	}
	void Node::handleRawEvents(const sf::Event& ev)
	{
		forEach([&](Node& n) {
			for (auto& comp : n.components)
				comp->handleRawEvents(ev);
		});
	}
	void Node::handleEvents(const EventHandler& ev, float delta)
	{
		forEach([&](Node& n) {
			for (auto& comp : n.components)
				comp->handleEvents(ev, delta);
			});
	}
	void Node::update(float delta)
	{
		forEach([&](Node& n) {
			for (auto& comp : n.components)
				comp->update(delta);
			});
	}
	void Node::draw(const SceneInfo& info) const
	{
		for (auto& comp : components)
			comp->draw(info);
	}
}