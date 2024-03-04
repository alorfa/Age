#pragma once

#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"

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
		void create(uvec2 mip0size, uint mipsCount);
		void use(const Texture& tex);
		inline const Texture& getResultTexture() const { return textures[0]; }
	};
}