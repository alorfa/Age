#include "FollowToCamera.hpp"
#include <Age/Transform/Camera3D.hpp>
#include <Age/Object/Node3D.hpp>

using namespace a_game_engine;

namespace a_game
{
	FollowToCamera::FollowToCamera(Node3D& node, const Camera3D& camera)
	{
		this->node = &node;
		this->camera = &camera;
	}

	void FollowToCamera::update(float delta)
	{
		node->transform.changePosition() = camera->transform.getPosition();
		node->transform.changeRotation() = camera->transform.getRotation();
	}
}

