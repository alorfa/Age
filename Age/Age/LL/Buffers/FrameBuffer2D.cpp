#include "FrameBuffer2D.hpp"
#include "Age/LL/opengl.h"
#include <utility>

namespace a_game_engine
{
	void FrameBuffer2D::clear()
	{
		if (_fbuf)
			glDeleteFramebuffers(1, &_fbuf);
		_fbuf = 0;
	}
	void FrameBuffer2D::clearRenderBuffer()
	{
		if (_rbuf)
			glDeleteRenderbuffers(1, &_rbuf);
		_rbuf = 0;
	}
	FrameBuffer2D::FrameBuffer2D()
	{
	}
	FrameBuffer2D::~FrameBuffer2D()
	{
		clear();
		clearRenderBuffer();
	}
	void FrameBuffer2D::createRenderBuffer(const uvec2& size)
	{
		glGenRenderbuffers(1, &_rbuf);
		glBindRenderbuffer(GL_RENDERBUFFER, _rbuf);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	}
	void FrameBuffer2D::create()
	{
		clear();
		glGenFramebuffers(1, &_fbuf);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbuf);

		for (uint i = 0; i < textures.size(); i++)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i].getId(), 0);

		if (_rbuf)
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbuf);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FrameBuffer2D::use()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbuf);
		glViewport(0, 0, textures[0].getSize().x, textures[0].getSize().y);
	}
	void FrameBuffer2D::useDefault(const uvec2& viewport)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, viewport.x, viewport.y);
	}
	bool FrameBuffer2D::checkActiveBuffer()
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}
	FrameBuffer2D::FrameBuffer2D(FrameBuffer2D&& other)
		: textures(std::move(other.textures))
	{
		std::swap(_fbuf, other._fbuf);
		std::swap(_rbuf, other._rbuf);
	}
	FrameBuffer2D& FrameBuffer2D::operator=(FrameBuffer2D&& other)
	{
		std::swap(textures, other.textures);
		std::swap(_fbuf, other._fbuf);
		std::swap(_rbuf, other._rbuf);
		return *this;
	}
}