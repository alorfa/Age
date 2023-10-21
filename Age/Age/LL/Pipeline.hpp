#pragma once

#include "Age/Math/vec3.hpp"

namespace a_game_engine
{
	enum class DepthFunc
	{
		Always, Never,
		Less, LEqual,
		Greater, GEqual,
		Equal, NotEqual,
		Disable = Always
	};
	enum class StencilOp
	{
		Keep, Zero, Replace, Incr, IncrWrap, Decr, DecrWrap, Invert
	};
	enum class CullFace
	{
		Disable, Back, Front, BackAndFront
	};
	enum class BlendMode
	{
		Lerp, Add, Disable, Default = Lerp
	};

	class Pipeline
	{
	public:
		static void setBlendMode(BlendMode mode);
		static void setDepthFunc(DepthFunc func);
		static void setFaceCulling(CullFace face);
		static void setFrontFace(bool ccw = true);
		static void setUnpackAlignment(int alignment = 1);
		static void clear(const vec3& color, bool depth = true, bool stencil = true);

		static void enableStencil(bool value = true);
		static void setStencilWrite(unsigned char value = 0xFF);
		static void setStencilFunc(DepthFunc func, unsigned char value, unsigned char mask = 0xFF);
		static void setStencilOp(StencilOp success, StencilOp fail = StencilOp::Keep);

		static void set3DContext();
		static void set2DContext();

		static int toOglValue(DepthFunc func);
		static int toOglValue(CullFace func);
		static int toOglValue(StencilOp op);
	};
}
