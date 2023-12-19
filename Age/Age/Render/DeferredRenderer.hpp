#pragma once

#include "Renderer.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/Scene/SceneInfo.hpp"

namespace a_game_engine
{
	class DeferredRenderer : public Renderer
	{
		FrameBuffer2D gbuffer, screenFb;

		const ShaderProgram 
			*debugPass,
			*pointLightPass, *dirLightPass, *spotLightPass,
			*postprocPass;
	public:
		int gbufferTime = 0, lightTime = 0, screenTime = 0;
		bool debug = false;

		DeferredRenderer();

		void updateSize(const uvec2& newSize);
		void drawObject(const Node& o, const SceneInfo& info) override;
		void drawLightSources(const Node& node, const vec3& cameraPos);
		void drawScene(const Scene& scene, const Camera& camera) override;
	};
}