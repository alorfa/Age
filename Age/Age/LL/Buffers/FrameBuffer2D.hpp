#pragma once

#include <Age/LL/Texture/Texture2D.hpp>
#include <vector>

namespace a_game_engine
{
	class FrameBuffer2D
	{
		uint _fbuf = 0, _rbuf = 0;
	public:
		std::vector<Texture2D> textures;

		FrameBuffer2D();
		FrameBuffer2D(const FrameBuffer2D&) = delete;
		FrameBuffer2D& operator=(const FrameBuffer2D&) = delete;
		FrameBuffer2D(FrameBuffer2D&&);
		FrameBuffer2D& operator=(FrameBuffer2D&&);
		~FrameBuffer2D();

		void createRenderBuffer(const uvec2& size);
		void clearRenderBuffer();
		void create();
		void clear();
		void use();
		static void useDefault(const uvec2& viewport);
		static bool checkActiveBuffer();
	};

	using FrameBuffer = FrameBuffer2D;
}