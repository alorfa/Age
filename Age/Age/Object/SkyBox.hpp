#pragma once

#include "Node3D.hpp"

namespace a_game_engine
{
	class Mesh3D;
	class CubeMap;

	class SkyBox : public Node3D
	{
	public:
		static const Mesh3D* cube;

		SkyBox(Node3D* parent);

		const CubeMap* cubemap = nullptr;

		void draw(const mat4& parent, const Node3D& scene, const Camera3D& c, const Shader* s) const override;
	};
}
