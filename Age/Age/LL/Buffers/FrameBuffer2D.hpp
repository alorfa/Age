#pragma once

#include <Age/LL/Texture/Texture2D.hpp>
#include <vector>

namespace a_game_engine
{
	class FrameBuffer2D
	{
		uint _fbuf = 0, _rbuf = 0;
	public:
		enum BufferType
		{
			Color = 1, Depth = 2, Stencil = 4
		};

		std::vector<Texture2D> textures;
		Texture2D depthStencil;

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
		void copyFrom(const FrameBuffer2D& fb, BufferType type, TextureFiltering filter = TextureFiltering::Near);
		static void useDefault(const uvec2& viewport);
		static bool checkActiveBuffer();
	};

	using FrameBuffer = FrameBuffer2D;
}