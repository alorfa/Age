#pragma once

#include "Renderer.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/Scene/SceneInfo.hpp"
#include "Age/Render/BloomEffect.hpp"
#include "Age/Scene/SkyBox.hpp"

namespace a_game_engine
{
	class ForwardRenderer : public Renderer
	{
		FrameBuffer2D mainFb;
		BloomEffect bloom;
		Texture2D colorBuffer, depthBuffer;
		float exposure = 1.f;

		ShaderProgram* shader;
	public:
		ForwardRenderer();

		const EnvCubeMap* env = nullptr;
		SkyBox skyBox;

		void clear() override;
		void updateSize(const uvec2& newSize);
		void drawScene(const Scene& scene, const Camera& camera, float delta) override;
	};
}