#pragma once

#include <Age/LL/Texture/Texture2D.hpp>

namespace a_game_engine
{
	class FrameBuffer2D
	{
		uint _fbuf = 0, _rbuf = 0;

		void clear();
	public:
		Texture2D texture;

		FrameBuffer2D();
		FrameBuffer2D(const FrameBuffer2D&) = delete;
		FrameBuffer2D& operator=(const FrameBuffer2D&) = delete;
		FrameBuffer2D(FrameBuffer2D&&);
		FrameBuffer2D& operator=(FrameBuffer2D&&);
		~FrameBuffer2D();

		struct Settings
		{
			uvec2 size;
			TextureFormat internal = TextureFormat::RGB_Float16, outer = TextureFormat::RGB_Float16;

			inline Settings(const uvec2& size, TextureFormat format = TextureFormat::RGB_Float16)
				: size(size), internal(format), outer(format) {}
			inline Settings(const uvec2& size, TextureFormat inner, TextureFormat outer)
				: size(size), internal(inner), outer(outer) {}
		};

		void create(const Settings& s);
		void use();
		static void useDefault(const uvec2& viewport);
		static bool checkActiveBuffer();
	};

	using FrameBuffer = FrameBuffer2D;
}