#pragma once

#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/LL/Pipeline.hpp"

namespace a_game_engine
{
	class BloomEffect
	{
		FrameBuffer2D fb;
		std::vector<Texture2D> textures;
		ShaderProgram *downscale, *upscale;
	public:
		float radius = 1.5f;
		int downscaleStartMipLevel = 0, downscaleEndMipLevel = -1;
		int upscaleStartMipLevel = 5, upscaleEndMipLevel = 0;
		float strength = 0.05f;
		BlendMode blendMode = BlendMode::Add;

		void destroy();
		void create(uvec2 mip0size, int mipsCount = 0);
		void useDownscale(const Texture& tex);
		void useUpscale();
		inline const std::vector<Texture2D>& getTextures() const { return textures; }
	};
}