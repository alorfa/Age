#pragma once

#include "Age/LL/Buffers/VertexBuffer.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/Material/Material.hpp"

namespace a_game_engine
{
	class Mesh3D
	{
	public:
		struct RenderInfo //not yed used
		{
			const TransformProps* transform = nullptr;
			const SceneProps* external = nullptr;
			const Shader* shader = nullptr;

			RenderInfo(const TransformProps& t, const SceneProps& e);
		};

		Mesh3D() = default;
		Mesh3D(const Mesh3D&) = delete;
		Mesh3D& operator=(const Mesh3D&) = delete;
		Mesh3D(Mesh3D&&) = default;
		Mesh3D& operator=(Mesh3D&&) = default;

		VertexBuffer buffer;
		Material material;

		void draw(const ShaderProgram& s) const;
	};
}