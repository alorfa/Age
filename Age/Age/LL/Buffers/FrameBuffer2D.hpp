#pragma once

#include <Age/LL/Texture/Texture2D.hpp>
#include <Age/LL/Texture/CubeMap.hpp>
#include <vector>

namespace a_game_engine
{
	class FrameBuffer2D
	{
		struct TexInfo
		{
			const Texture2D* tex2d = nullptr;
			const CubeMap* cubemap = nullptr;
			int face = 0;
		};

		uint _fbuf = 0, _rbuf = 0;
		uvec2 _rbufSize;
		int _mipLevel = 0;
		std::vector<TexInfo> _textures;
		const Texture2D* _depthStencil = nullptr;

		void create();
		uvec2 getSize() const;
	public:
		enum BufferType
		{
			Color = 1, Depth = 2, Stencil = 4
		};

		FrameBuffer2D();
		FrameBuffer2D(const FrameBuffer2D&) = delete;
		FrameBuffer2D& operator=(const FrameBuffer2D&) = delete;
		FrameBuffer2D(FrameBuffer2D&&);
		FrameBuffer2D& operator=(FrameBuffer2D&&);
		~FrameBuffer2D();

		void setTexturesCount(uint count);
		void setTexture(uint index, const Texture& t, int mipLevel = 0);
		void setCubemapTexture(uint index, const CubeMap& t, CubeMap::Face face, int mipLevel = 0);
		void removeTexture(uint index, int mipLevel = 0);
		void setDepthTexture(const Texture& t);
		void removeDepthTexture();
		void createRenderBuffer(const uvec2& size, TextureFormat format = TextureFormat::Depth24_Stencil8);
		void removeRenderBuffer();
		void clear();
		void use();
		void copyFrom(const FrameBuffer2D& fb, int type, TextureFiltering filter = TextureFiltering::Near);
		static void useDefault(const uvec2& viewport);
		bool isValid() const;
	};

	using FrameBuffer = FrameBuffer2D;
}
