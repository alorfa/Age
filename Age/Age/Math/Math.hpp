#pragma once

#include "vec3.hpp"

namespace a_game_engine
{
	class Math
	{
	public:
		static const float PI;
		static const float TAU;

		static float sin(float value);
		static float cos(float value);

		static float rad(float deg);
		static float deg(float rad);

		static vec3 getForwardDir(const vec3& euler);
		static vec3 getRightDir(const vec3& euler);
		static vec3 getUpDir(const vec3& euler);

		template <typename T>
		static T max(T v1, T v2)
		{
			if (v1 > v2)
				return v1;
			return v2;
		}
		template <typename T>
		static T min(T v1, T v2)
		{
			if (v1 < v2)
				return v1;
			return v2;
		}

		template <typename T>
		static T clamp(T value, T min, T max)
		{
			if (value < min)
				return min;
			if (value > max)
				return max;
			return value;
		}
		static float saturate(float value);

		template <typename T>
		static T lerp(T first, T second, float t)
		{
			return first + (second - first) * t;
		}
		template <typename T>
		static T smooth(T first, T second, float t)
		{
			return lerp(first, second, saturate(t));
		}

		static vec3 cross(const vec3& v1, const vec3& v2);
	};
}
