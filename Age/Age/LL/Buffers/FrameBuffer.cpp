#include "FrameBuffer.hpp"
#include "Age/LL/opengl.h"

namespace a_game_engine
{
	void FrameBuffer2D::clear()
	{
		if (_fbuf)
			glDeleteFramebuffers(1, &_fbuf);
		if (_rbuf)
			glDeleteRenderbuffers(1, &_rbuf);
		_fbuf = _rbuf = 0;
	}
	FrameBuffer2D::FrameBuffer2D()
	{
	}
	FrameBuffer2D::~FrameBuffer2D()
	{
		clear();
	}
	void FrameBuffer2D::create(const Settings& s)
	{
		clear();
		glGenFramebuffers(1, &_fbuf);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbuf);
		texture.create(Texture2D::Settings{s.size, nullptr, s.format, 
			s.hdr ? TextureDataType::Float : TextureDataType::Ubyte, false});
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getId(), 0);

		glGenRenderbuffers(1, &_rbuf);
		glBindRenderbuffer(GL_RENDERBUFFER, _rbuf); 
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, s.size.x, s.size.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbuf);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FrameBuffer2D::use()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbuf);
		glViewport(0, 0, texture.getSize().x, texture.getSize().y);
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
		//: _tex(std::move(other._tex))
	{
	}
	FrameBuffer2D& FrameBuffer2D::operator=(FrameBuffer2D&& other)
	{
		return *this;
	}
	FrameBuffer2D::Settings::Settings(const uvec2& size, TextureFormat format, bool hdr)
		: size(size), format(format), hdr(hdr)
	{}
}