#pragma once

#include "Age/LL/Buffers/FrameBuffer2D.hpp"

namespace a_game_engine
{
	class BloomEffect
	{
		FrameBuffer2D fb;
		std::vector<Texture2D> textures;
	public:
		void create(uvec2 mip0size, uint mipsCount);
	};
}