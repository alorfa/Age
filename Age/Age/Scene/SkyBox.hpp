#pragma once

namespace a_game_engine
{
	class VertexBuffer;
	class CubeMap;
	class ShaderProgram;
	class Camera;
	class Scene;

	class SkyBox
	{
	public:
		static const VertexBuffer* cube;

		const ShaderProgram* shader = nullptr;
		const CubeMap* cubemap = nullptr;

		void draw(const Camera& c, const ShaderProgram* s) const;
	};
}
