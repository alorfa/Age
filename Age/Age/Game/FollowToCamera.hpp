#pragma once

#include <Age/Object/Component.hpp>

namespace a_game_engine
{
	class Node3D;
	class Camera3D;
}

using namespace a_game_engine;

namespace a_game
{
	class FollowToCamera : public Component
	{
		Node3D* node = nullptr;
		const Camera3D* camera = nullptr;
	public:
		FollowToCamera(Node3D& node, const Camera3D& camera);

		void update(float delta) override;
	};
}