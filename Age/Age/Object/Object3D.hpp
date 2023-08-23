#pragma once

#include "Node3D.hpp"

namespace a_game_engine
{
	class Shader;
	class Model3D;

	class Object3D : public Node3D
	{
	public:
		Object3D(Node3D* parent);

		const Model3D* model;

		virtual void draw(const mat4& parent, const Node3D& scene, const Camera3D& c,
			const Shader* s = nullptr) const;
	};
}
