#include "Pipeline.hpp"
#include "opengl.h"

namespace a_game_engine
{
	namespace
	{
		DepthFunc depthFunc = DepthFunc::Always;
		CullFace cullFace = CullFace::Disable;
		bool frontFaceCCW = true;
		BlendMode blendNode = BlendMode::Disable;

		bool stencilIsEnabled = false;
		DepthFunc stencilFunc = DepthFunc::Always;
		unsigned char stencilWrite = 0, stencilMask = 0, stencilTest = 0;
	}

	void Pipeline::setBlendMode(BlendMode mode)
	{
		if (blendNode == mode)
			return;

		if (mode == BlendMode::Disable)
			glDisable(GL_BLEND);
		else
			glEnable(GL_BLEND);

		switch (mode)
		{
		case BlendMode::Lerp:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BlendMode::Add:
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		}
		blendNode = mode;
	}

	void Pipeline::setDepthFunc(DepthFunc func)
	{
		if (func != depthFunc)
		{
			if (depthFunc == DepthFunc::Always)
				glEnable(GL_DEPTH_TEST);
			depthFunc = func;
			if (func == DepthFunc::Always)
			{
				glDisable(GL_DEPTH_TEST);
				return;
			}

			glDepthFunc(toOglValue(func));
		}
	}
	void Pipeline::setFaceCulling(CullFace face)
	{
		if (face != cullFace)
		{
			if (cullFace == CullFace::Disable)
				glEnable(GL_CULL_FACE);
			cullFace = face;
			if (face == CullFace::Disable)
			{
				glDisable(GL_CULL_FACE);
				return;
			}

			glCullFace(toOglValue(face));
		}
	}
	void Pipeline::setFrontFace(bool ccw)
	{
		if (frontFaceCCW != ccw)
		{
			if (ccw)
				glFrontFace(GL_CCW);
			else
				glFrontFace(GL_CW);
		}
	}
	void Pipeline::setUnpackAlignment(int alignment)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	}
	void Pipeline::clear(const vec3& color, bool depth, bool stencil)
	{
		int flag = GL_COLOR_BUFFER_BIT;
		if (depth)
			flag |= GL_DEPTH_BUFFER_BIT;
		if (stencil)
			flag |= GL_STENCIL_BUFFER_BIT;
		glClearColor(color.x, color.y, color.z, 1.f);
		glClear(flag);
	}
	void Pipeline::enableStencil(bool value)
	{
		if (value == stencilIsEnabled)
			return;

		stencilIsEnabled = value;
		if (value)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
	}
	void Pipeline::setStencilWrite(unsigned char value)
	{
		if (value != stencilWrite)
		{
			glStencilMask(value);
			stencilWrite = value;
		}
	}
	void Pipeline::setStencilFunc(DepthFunc func, unsigned char value, unsigned char mask)
	{
		if (stencilFunc == func && stencilTest == value && stencilMask == mask)
			return;

		stencilFunc = func;
		stencilTest = value;
		stencilMask = mask;

		glStencilFunc(toOglValue(func), value, mask);
	}
	void Pipeline::setStencilOp(StencilOp success, StencilOp fail)
	{
		const int failValue = toOglValue(fail);
		glStencilOp(failValue, failValue, toOglValue(success));
	}
	void Pipeline::set3DContext()
	{
		setDepthFunc(DepthFunc::Less);
		setFaceCulling(CullFace::Back);
	}
	void Pipeline::set2DContext()
	{
		setDepthFunc(DepthFunc::Disable);
		setFaceCulling(CullFace::Disable);
	}
	int Pipeline::toOglValue(DepthFunc func)
	{
		switch (func)
		{
		case DepthFunc::Always:
			return GL_ALWAYS;
		case DepthFunc::Never:
			return GL_NEVER;
		case DepthFunc::Less:
			return GL_LESS;
		case DepthFunc::LEqual:
			return GL_LEQUAL;
		case DepthFunc::Greater:
			return GL_GREATER;
		case DepthFunc::GEqual:
			return GL_GEQUAL;
		case DepthFunc::Equal:
			return GL_EQUAL;
		case DepthFunc::NotEqual:
			return GL_NOTEQUAL;
		}
		return GL_LESS;
	}
	int Pipeline::toOglValue(CullFace func)
	{
		switch (func)
		{
		case CullFace::Back:
			return GL_BACK;
		case CullFace::Front:
			return GL_FRONT;
		case CullFace::BackAndFront:
			return GL_FRONT_AND_BACK; 
		}
		return GL_BACK;
	}
	int Pipeline::toOglValue(StencilOp op)
	{
		switch (op)
		{
		case StencilOp::Keep:
			return GL_KEEP;
		case StencilOp::Zero:
			return GL_ZERO;
		case StencilOp::Replace:
			return GL_REPLACE;
		case StencilOp::Incr:
			return GL_INCR;
		case StencilOp::IncrWrap:
			return GL_INCR_WRAP;
		case StencilOp::Decr:
			return GL_DECR;
		case StencilOp::DecrWrap:
			return GL_DECR_WRAP;
		case StencilOp::Invert:
			return GL_INVERT;
		}
		return GL_KEEP;
	}
}