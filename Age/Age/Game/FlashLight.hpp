#pragma once

#include "Age/Components/LightComponents.hpp"

namespace a_game_engine
{
	class Node;
	class Camera;
}

using namespace a_game_engine;

namespace a_game
{
	class FlashLight : public Component
	{
		Node* node = nullptr;
		const Camera* camera = nullptr;

		SpotLightComponent* spotLight = nullptr;
	public:
		FlashLight(Node& node, const Camera& camera);
		inline FlashLight(Node& node)
			: node(&node) {}

		inline FlashLight& setCamera(const Camera& camera) {
			this->camera = &camera;
			return *this;
		}

		void handleEvents(const EventHandler& ev, float delta) override;
		void update(float delta) override;
	};
}