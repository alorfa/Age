#include "FollowToCamera.hpp"
#include <Age/Transform/Camera.hpp>
#include <Age/Object/Node.hpp>

using namespace a_game_engine;

namespace a_game
{
	FollowToCamera::FollowToCamera(Node& node, const Camera& camera)
	{
		this->node = &node;
		this->camera = &camera;
	}

	void FollowToCamera::update(float delta)
	{
		node->setPosition(camera->transform.getPosition());
		node->setEulerRotation(camera->transform.getRotation());
	}
}

