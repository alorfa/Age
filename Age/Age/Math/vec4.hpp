#pragma once

#include <math.h>

namespace a_game_engine
{
	template <typename T>
	struct vector4
	{
		T x, y, z, w;

		vector4()
		{
			x = y = z = w = 0;
		}
		vector4(T value)
		{
			x = y = z = w = value;
		}
		vector4(T vx, T vy, T vz, T vw)
		{
			x = vx;
			y = vy;
			z = vz;
			w = vw;
		}

		float square_length() const
		{
			return x * x + y * y + z * z + w * w;
		}
		float length() const
		{
			return sqrtf(square_length());
		}

		vector4 new_normalized() const
		{
			return (*this) * (1.f / length());
		}
		void normalize()
		{
			float l = 1.f / length();
			(*this) *= l;
		}
		vector4 operator-() const
		{
			return vector4(-x, -y, -z, -w);
		}

		bool operator==(const vector4& v) const
		{
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}
		bool operator!=(const vector4& v) const
		{
			return !operator==(v);
		}
		static float dot(const vector4& v1, const vector4& v2)
		{
			return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
		}
	};

#define AGE_VECTOR4_OPERATOR_IMPL(oper)										\
	template <typename type>												\
	vector4<type> operator oper(vector4<type> v1, vector4<type> v2)			\
	{																		\
		return vector4<type>(v1.x oper v2.x, v1.y oper v2.y, v1.z oper v2.z, v1.w oper v2.w);\
	}																		\
	template <typename type>												\
	vector4<type>& operator oper##=(vector4<type>& v1, vector4<type> v2)	\
	{																		\
		v1.x oper##= v2.x;													\
		v1.y oper##= v2.y;													\
		v1.z oper##= v2.z;													\
		v1.w oper##= v2.w;													\
		return v1;															\
	}

#define AGE_SCALAR4_OPERATOR_IMPL(oper)										\
	template <typename type>												\
	vector4<type> operator oper(vector4<type> v1, type value)				\
	{																		\
		return vector4<type>(v1.x oper value, v1.y oper value, v1.z oper value, v1.w oper value);\
	}																		\
	template <typename type>												\
	vector4<type>& operator oper##=(vector4<type>& v1, type value)			\
	{																		\
		v1.x oper##= value;													\
		v1.y oper##= value;													\
		v1.z oper##= value;													\
		v1.w oper##= value;													\
		return v1;															\
	}

	AGE_VECTOR4_OPERATOR_IMPL(+);
	AGE_VECTOR4_OPERATOR_IMPL(-);
	AGE_VECTOR4_OPERATOR_IMPL(*);
	AGE_VECTOR4_OPERATOR_IMPL(/);

	AGE_SCALAR4_OPERATOR_IMPL(+);
	AGE_SCALAR4_OPERATOR_IMPL(-);
	AGE_SCALAR4_OPERATOR_IMPL(*);
	AGE_SCALAR4_OPERATOR_IMPL(/);

	using fvec4 = vector4<float>;
	using dvec4 = vector4<double>;
	using vec4 = fvec4;

	using ivec4 = vector4<int>;
	using uivec4 = vector4<unsigned>;
	using uvec4 = uivec4;

	using vec4_size = vector4<size_t>;
}