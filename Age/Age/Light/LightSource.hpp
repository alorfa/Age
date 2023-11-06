#pragma once

#include "Light.hpp"
#include "Age/Object/Object3D.hpp"

namespace a_game_engine
{
	class Scene;

	class PointLightSource : public Object3D
	{
	public:
		PointLight light;

		PointLightSource(Scene& scene, Node* parent);
	};

	class SpotLightSource : public Object3D
	{
	public:
		SpotLight light;

		SpotLightSource(Scene& scene, Node* parent);
	};

	class DirLightSource : public Node
	{
	public:
		DirLight light;

		DirLightSource(Scene& scene, Node* parent);
	};
}