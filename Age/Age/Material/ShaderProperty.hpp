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
			int slot = -1;

			inline Texture2DProp(const Texture2D& tex, int slot = -1)
				: texture(&tex), slot(slot) {}
		};
		struct CubeMapProp
		{
			const CubeMap* cubemap = nullptr;
			int slot = -1;

			inline CubeMapProp(const CubeMap& tex, int slot = -1)
				: cubemap(&tex), slot(slot) {}
		};
		using PropertyValue = std::variant<int, float, vec2, vec3, mat3, mat4, Texture2DProp, CubeMapProp>;

		PropertyValue value;
		Type getType() const {
			return (Type)(value.index());
		}
	};
}