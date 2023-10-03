#pragma once

#include "Renderer3D.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/Scene/Scene3DInfo.hpp"

namespace a_game_engine
{
	class DeferredRenderer : public Renderer3D
	{
		mutable FrameBuffer2D gbuffer, screenFb;

		const ShaderProgram 
			*debugPass,
			*pointLightPass, *dirLightPass, *spotLightPass,
			*postprocPass;

	public:
		DeferredRenderer();

		void updateSize(const uvec2& newSize);
		void drawObject(const Node3D& o, const Scene3DInfo& info) override;
		void drawLightSources(const Node3D& node, const vec3& cameraPos);
		void drawScene(const Scene3D& scene, const Camera3D& camera) override;
	};
}