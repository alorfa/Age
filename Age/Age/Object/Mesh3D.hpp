#pragma once

#include "Age/LL/Buffers/VertexBuffer.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/Object/Material.hpp"

namespace a_game_engine
{
	class Mesh3D
	{
	public:
		Mesh3D() = default;
		Mesh3D(const Mesh3D&) = delete;
		Mesh3D& operator=(const Mesh3D&) = delete;
		Mesh3D(Mesh3D&&) = default;
		Mesh3D& operator=(Mesh3D&&) = default;

		VertexBuffer buffer;
		TextureMaterial material;

		void draw(const ShaderProgram& s, uint textureOrder) const;
	};
}