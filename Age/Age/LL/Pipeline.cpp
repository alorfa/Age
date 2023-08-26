#include "Pipeline.hpp"
#include "opengl.h"

namespace a_game_engine
{
	namespace
	{
		DepthFunc depthFunc = DepthFunc::Never;
		CullFace cullFace = CullFace::Disable;
		bool frontFaceCCW = true;
	}

	void Pipeline::setDepthFunc(DepthFunc func)
	{
		if (func != depthFunc)
		{
			if (depthFunc == DepthFunc::Never)
				glEnable(GL_DEPTH_TEST);
			depthFunc = func;
			if (func == DepthFunc::Never)
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
	void Pipeline::clear(const vec3& color)
	{
		int flag = GL_COLOR_BUFFER_BIT;
		if (depthFunc != DepthFunc::Disable)
			flag |= GL_DEPTH_BUFFER_BIT;
		glClearColor(color.x, color.y, color.z, 1.f);
		glClear(flag);
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
}