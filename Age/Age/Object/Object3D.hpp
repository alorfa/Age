#pragma once

#include "Node3D.hpp"

namespace a_game_engine
{
	class ShaderProgram;
	class Model3D;
	class Scene3D;

	class Object3D : public Node3D
	{
	public:
		Object3D(Scene3D& scene, Node3D* parent, Node3D::Type type = Node3D::Usual);

		const Model3D* model;

		virtual void draw(const mat4& parent, const Scene3DInfo& info) const override;
	};
}
