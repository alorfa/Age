#pragma once

#include <variant>
#include "Age/Math/vec2.hpp"
#include "Age/Math/vec3.hpp"
#include "Age/Math/mat3.hpp"
#include "Age/Math/mat4.hpp"

namespace a_game_engine
{
	class Texture2D;
	class CubeMap;

	class ShaderProperty
	{
	public:
		enum class Type
		{
			Int, Float, Vec2, Vec3, Mat3, Mat4, Texture2D, CubeMap
		};
		struct Texture2DProp
		{
			const Texture2D* texture = nullptr;
			unsigned slot = 0;
		};
		struct CubeMapProp
		{
			const CubeMap* cubemap = nullptr;
			unsigned slot = 0;
		};
		using PropertyValue = std::variant<int, float, vec2, vec3, mat3, mat4, Texture2DProp, CubeMapProp>;

		PropertyValue value;
		Type getType() const {
			return (Type)(value.index());
		}
	};
}