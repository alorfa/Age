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
		float radius = 1.f;

		void destroy();
		void create(uvec2 mip0size, int mipsCount = 0);
		void useDownscale(const Texture& tex, int dstMip = 1, int lastMipLevel = 0);
		void useUpscale(int mipStart, int mipEnd, BlendMode mode = BlendMode::Add);
		inline const std::vector<Texture2D>& getTextures() const { return textures; }
	};
}