#pragma once

#include "Age/LL/Buffers/VertexBuffer.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/Material/Material.hpp"
#include "Age/Material/ShaderSettings.hpp"

namespace a_game_engine
{
	class Mesh
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

		Mesh() = default;
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&&) = default;
		Mesh& operator=(Mesh&&) = default;

		const VertexBuffer* buffer = nullptr;
		Material material;

		void draw(const RenderInfo& info) const;
	};
}