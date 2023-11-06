#pragma once

namespace a_game_engine
{
	class Mesh;
	class CubeMap;
	class ShaderProgram;
	class Camera;
	class Scene;

	class SkyBox
	{
	public:
		static const Mesh* cube;

		const ShaderProgram* shader = nullptr;
		const CubeMap* cubemap = nullptr;
		Scene* const scene = nullptr;

		SkyBox(Scene& scene);

		void draw(const Camera& c, const ShaderProgram* s) const;

		static int getSlot();
	};
}
