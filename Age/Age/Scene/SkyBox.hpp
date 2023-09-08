#pragma once

namespace a_game_engine
{
	class Mesh3D;
	class CubeMap;
	class Shader;
	class Camera3D;
	class Scene3D;

	class SkyBox
	{
	public:
		static const Mesh3D* cube;

		const Shader* shader = nullptr;
		const CubeMap* cubemap = nullptr;
		Scene3D* const scene = nullptr;

		SkyBox(Scene3D& scene);

		void draw(const Camera3D& c, const Shader* s) const;

		static int getSlot();
	};
}