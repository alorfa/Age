#pragma once

#include <Age/Object/Component.hpp>

namespace a_game_engine
{
	class Node;
	class Camera;
}

using namespace a_game_engine;

namespace a_game
{
	class FollowToCamera : public Component
	{
		Node* node = nullptr;
		const Camera* camera = nullptr;
	public:
		FollowToCamera(Node& node, const Camera& camera);

		void update(float delta) override;
	};
}