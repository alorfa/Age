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
		Disable = Never
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
		static void clear(const vec3& color);

		static void set3DContext();
		static void set2DContext();

		static int toOglValue(DepthFunc func);
		static int toOglValue(CullFace func);
	};
}
