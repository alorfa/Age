#include "FrameBuffer2D.hpp"
#include "Age/LL/opengl.h"
#include <utility>

namespace a_game_engine
{
	namespace
	{
		const GLenum attachments[] =
		{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
	}

	void FrameBuffer2D::create()
	{
		if (_fbuf == 0)
			glCreateFramebuffers(1, &_fbuf);
	}
	uvec2 FrameBuffer2D::getSize() const
	{
		if (_depthStencil)
			return _depthStencil->getSize();

		if (_rbufSize.x && _rbufSize.y)
			return _rbufSize;

		if (_textures[0].tex2d)
			return _textures[0].tex2d->getSize();

		if (_textures[0].cubemap)
			return uvec2(_textures[0].cubemap->getSize());

		return {};
	}

	void FrameBuffer2D::clear()
	{
		if (_fbuf)
			glDeleteFramebuffers(1, &_fbuf);
		_fbuf = 0;
		_depthStencil = nullptr;
		for (auto& t : _textures)
			t = TexInfo{};
	}
	FrameBuffer2D::FrameBuffer2D()
	{
		setTexturesCount(1);
	}
	FrameBuffer2D::~FrameBuffer2D()
	{
		clear();
		removeRenderBuffer();
	}
	void FrameBuffer2D::setTexturesCount(uint count)
	{
		_textures.resize(count);
	}
	void FrameBuffer2D::setTexture(uint index, const Texture& t, int mipLevel)
	{
		create();

		TexInfo info;
		info.tex2d = &t;
		_textures[index] = info;
		_mipLevel = mipLevel;

		glNamedFramebufferTexture(_fbuf, GL_COLOR_ATTACHMENT0 + index, t.getId(), mipLevel);
	}
	void FrameBuffer2D::setCubemapTexture(uint index, const CubeMap& t, CubeMap::Face face, int mipLevel)
	{
		create();

		TexInfo info;
		info.cubemap = &t;
		_textures[index] = info;
		_mipLevel = mipLevel;

		glNamedFramebufferTexture2DEXT(
			_fbuf, GL_COLOR_ATTACHMENT0 + index, 
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, t.getId(), mipLevel);
	}
	void FrameBuffer2D::removeTexture(uint index, int mipLevel)
	{
		create();
		
		if (_textures[index].tex2d)
			glNamedFramebufferTexture(_fbuf, GL_COLOR_ATTACHMENT0 + index, 0, mipLevel);
		if (_textures[index].cubemap)
			glNamedFramebufferTexture2DEXT(
				_fbuf, GL_COLOR_ATTACHMENT0 + index,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + _textures[index].face, 0, mipLevel);
		_textures[index] = TexInfo{};
		_mipLevel = mipLevel;
	}
	void FrameBuffer2D::setDepthTexture(const Texture& t)
	{
		create();

		const bool hasStencil = t.getFormat() == TextureFormat::Depth24_Stencil8;
		const int attachment = hasStencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
		_depthStencil = &t;
		glNamedFramebufferTexture(_fbuf, attachment, t.getId(), 0);
	}
	void FrameBuffer2D::removeDepthTexture()
	{
		if (_depthStencil == nullptr)
			return;

		create();
		const bool hasStencil = _depthStencil->getFormat() == TextureFormat::Depth24_Stencil8;
		const int attachment = hasStencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
		glNamedFramebufferTexture(_fbuf, attachment, 0, 0);
		_depthStencil = nullptr;
	}
	void FrameBuffer2D::createRenderBuffer(const uvec2& size, TextureFormat format)
	{
		create();
		removeRenderBuffer();
		const bool hasStencil = format == TextureFormat::Depth24_Stencil8;
		const int attachment = hasStencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
		_rbufSize = size;
		glCreateRenderbuffers(1, &_rbuf);
		glNamedRenderbufferStorage(_rbuf, TexEnums::toOglFormat(format), size.x, size.y);
		glNamedFramebufferRenderbuffer(_fbuf, attachment, GL_RENDERBUFFER, _rbuf);
	}
	void FrameBuffer2D::removeRenderBuffer()
	{
		if (_rbuf)
			glDeleteRenderbuffers(1, &_rbuf);
		_rbuf = 0;
		_rbufSize = uvec2{};
	}
	void FrameBuffer2D::use()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbuf);
		glDrawBuffers((int)_textures.size(), attachments);
		uvec2 size = getSize();

		for (int i = 0; i < _mipLevel; i++)
		{
			size /= 2u;
		}
		glViewport(0, 0, size.x, size.y);
	}
	void FrameBuffer2D::copyFrom(const FrameBuffer2D& fb, int type, TextureFiltering filter)
	{
		uvec2 dstSize = getSize();
		uvec2 srcSize = fb.getSize();

		if (srcSize.x == 0 or srcSize.y == 0 or
			dstSize.x == 0 or dstSize.y == 0)
		{
			return;
		}

		int bitmask = 0;
		if (type & BufferType::Color)
			bitmask |= GL_COLOR_BUFFER_BIT;
		if (type & BufferType::Depth)
			bitmask |= GL_DEPTH_BUFFER_BIT;
		if (type & BufferType::Stencil)
			bitmask |= GL_STENCIL_BUFFER_BIT;

		glBlitNamedFramebuffer(fb._fbuf, _fbuf,
			0, 0, srcSize.x, srcSize.y,
			0, 0, dstSize.x, dstSize.y,
			bitmask, TexEnums::toOglFilter(filter));
	}
	void FrameBuffer2D::useDefault(const uvec2& viewport)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, viewport.x, viewport.y);
	}
	bool FrameBuffer2D::isValid() const
	{
		return glCheckNamedFramebufferStatus(_fbuf, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}
	FrameBuffer2D::FrameBuffer2D(FrameBuffer2D&& other)
	{
		operator=(std::move(other));
	}
	FrameBuffer2D& FrameBuffer2D::operator=(FrameBuffer2D&& other)
	{
		std::swap(_textures, other._textures);
		std::swap(_depthStencil, other._depthStencil);
		std::swap(_fbuf, other._fbuf);
		std::swap(_rbuf, other._rbuf);
		std::swap(_rbufSize, other._rbufSize);
		std::swap(_mipLevel, other._mipLevel);
		return *this;
	}
}