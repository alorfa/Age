#include "Rotate.hpp"
#include "Age/Object/Node.hpp"

namespace a_game
{
	Rotate::Rotate(Node& node)
		: node(&node)
	{
	}

	void Rotate::update(float delta)
	{
		node->changeTransform().changeRotation().z += delta * 0.2f;
	}
}

