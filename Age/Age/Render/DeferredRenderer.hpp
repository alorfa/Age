#pragma once

#include "Renderer.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/Scene/SceneInfo.hpp"
#include "Age/Scene/SkyBox.hpp"
#include "Age/Render/BloomEffect.hpp"

namespace a_game_engine
{
	class DeferredRenderer : public Renderer
	{
		FrameBuffer2D gbuffer, screenFb;

		Texture2D albedoRoughnessMap, normalMetalnessMap, posMap;
		Texture2D screenBuffer, depthBuffer;

		const ShaderProgram 
			*debugPass,
			*pointLightPass, *dirLightPass, *spotLightPass, *iblPass, *shadowDirLightPass,
			*postprocPass;
		float exposure = 1.f;
	public:
		int gbufferTime = 0, lightTime = 0, screenTime = 0;
		bool debug = false;

		const EnvCubeMap* env = nullptr;
		SkyBox skyBox;
		BloomEffect* bloom = nullptr;
		vec3 fogColor;
		float fogDistance = 0.f;

		DeferredRenderer();

		void clear() override;
		void updateSize(const uvec2& newSize);
		void drawLightSources(const Node& node, const vec3& cameraPos);
		void drawScene(const Scene& scene, const Camera& camera, float delta) override;
	};
}