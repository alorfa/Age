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
			glGenFramebuffers(1, &_fbuf);
	}
	uvec2 FrameBuffer2D::getSize() const
	{
		if (_depthStencil)
			return _depthStencil->getSize();

		if (_rbufSize.x && _rbufSize.y)
			return _rbufSize;

		if (_textures[0])
			return _textures[0]->getSize();

		return {};
	}

	void FrameBuffer2D::clear()
	{
		if (_fbuf)
			glDeleteFramebuffers(1, &_fbuf);
		_fbuf = 0;
		_depthStencil = nullptr;
		for (auto& t : _textures)
			t = nullptr;
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

		_textures[index] = &t;

		glBindFramebuffer(GL_FRAMEBUFFER, _fbuf);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, t.getId(), mipLevel);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FrameBuffer2D::removeTexture(uint index, int mipLevel)
	{
		create();
		
		glBindFramebuffer(GL_FRAMEBUFFER, _fbuf);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, 0, mipLevel);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		_textures[index] = nullptr;
	}
	void FrameBuffer2D::setDepthTexture(const Texture& t)
	{
		create();

		const bool hasStencil = t.getFormat() == TextureFormat::Depth24_Stencil8;
		const int attachment = hasStencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
		_depthStencil = &t;
		glBindFramebuffer(GL_FRAMEBUFFER, _fbuf);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, t.getId(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FrameBuffer2D::removeDepthTexture()
	{
		if (_depthStencil == nullptr)
			return;

		create();
		const bool hasStencil = _depthStencil->getFormat() == TextureFormat::Depth24_Stencil8;
		const int attachment = hasStencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
		glBindFramebuffer(GL_FRAMEBUFFER, _fbuf);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, 0, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		_depthStencil = nullptr;
	}
	void FrameBuffer2D::createRenderBuffer(const uvec2& size, TextureFormat format)
	{
		create();
		removeRenderBuffer();
		const bool hasStencil = format == TextureFormat::Depth24_Stencil8;
		const int attachment = hasStencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
		_rbufSize = size;
		glGenRenderbuffers(1, &_rbuf);
		glBindRenderbuffer(GL_RENDERBUFFER, _rbuf);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbuf);
		glRenderbufferStorage(GL_RENDERBUFFER, TexEnums::toOglFormat(format), size.x, size.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, _rbuf);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		uvec2 size;
		if (_textures[0] == nullptr)
			size = _depthStencil->getSize();
		else
			size = _textures[0]->getSize();
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
		//glDrawBuffers(1, attachments);
		glViewport(0, 0, viewport.x, viewport.y);
	}
	bool FrameBuffer2D::checkActiveBuffer()
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}
	FrameBuffer2D::FrameBuffer2D(FrameBuffer2D&& other)
		: _textures(std::move(other._textures))
	{
		std::swap(_fbuf, other._fbuf);
		std::swap(_rbuf, other._rbuf);
		std::swap(_depthStencil, other._depthStencil);
		std::swap(_rbufSize, other._rbufSize);
	}
	FrameBuffer2D& FrameBuffer2D::operator=(FrameBuffer2D&& other)
	{
		std::swap(_textures, other._textures);
		std::swap(_depthStencil, other._depthStencil);
		std::swap(_fbuf, other._fbuf);
		std::swap(_rbuf, other._rbuf);
		std::swap(_rbufSize, other._rbufSize);
		return *this;
	}
}