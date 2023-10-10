#include "Rotate.hpp"
#include "Age/Object/Node3D.hpp"

namespace a_game
{
	Rotate::Rotate(Node3D& node)
		: node(&node)
	{
	}

	void Rotate::update(float delta)
	{
		node->transform.changeRotation().z += delta * 0.2f;
	}
}

