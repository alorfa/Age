#pragma once

#include <Age/Scene/DefaultRenderer3D.hpp>
#include <Age/Object/Node3D.hpp>
#include <Age/Light/LightSource.hpp>

using namespace a_game_engine;

namespace a_game
{
	class WorldScene : public Node3D
	{
		DefaultRenderer3D defRender;
		DirLightSource* dirLight;
		SpotLightSource* spotLight;
	public:
		WorldScene();

		Camera3D* activeCamera;

		float time = 0.f;

		void load();
		void handleRawEvents(const sf::Event& ev) override;
		void handleEvents(const EventHandler& ev, float delta) override;
		void update(float delta) override;
		void draw(const mat4& transform, const Node3D&, const Camera3D&, const Shader* s) const override;
	};
}