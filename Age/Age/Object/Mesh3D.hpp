#pragma once

#include "Age/LL/Buffers/VertexBuffer.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/Material/Material.hpp"
#include "Age/Material/ShaderSettings.hpp"

namespace a_game_engine
{
	class Mesh3D
	{
	public:
		struct RenderInfo
		{
			const TransformProps* transform = nullptr;
			const SceneProps* external = nullptr;
			const Shader* shader = nullptr;
			const ShaderSettings::Common* shaderSettings = nullptr;

			inline RenderInfo(
				const TransformProps& t,
				const SceneProps& e,
				const Shader& s,
				const ShaderSettings::Common& settings)
				: transform(&t), external(&e), shader(&s), shaderSettings(&settings) {}
		};

		Mesh3D() = default;
		Mesh3D(const Mesh3D&) = delete;
		Mesh3D& operator=(const Mesh3D&) = delete;
		Mesh3D(Mesh3D&&) = default;
		Mesh3D& operator=(Mesh3D&&) = default;

		VertexBuffer buffer;
		Material material;

		void draw(const RenderInfo& info) const;
	};
}