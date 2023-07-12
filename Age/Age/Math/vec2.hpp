#pragma once

namespace a_game_engine
{
	template <typename T>
	struct vector2
	{
		T x, y;

		vector2()
		{
			x = y = 0;
		}
		vector2(T value)
		{
			x = y = value;
		}
		vector2(T vx, T vy)
		{
			x = vx;
			y = vy;
		}

		float square_length() const
		{
			return x * x + y * y;
		}
		float length() const
		{
			return sqrtf(square_length());
		}

		vector2 new_normalized() const
		{
			return (*this) / length();
		}
		void normalize()
		{
			float l = 1.f / length();
			(*this) *= l;
		}
	};

#define AGE_VECTOR2_OPERATOR_IMPL(oper)										\
	template <typename type>												\
	vector2<type> operator oper(vector2<type> v1, vector2<type> v2)			\
	{																		\
		return vector2<type>(v1.x oper v2.x, v1.y oper v2.y);				\
	}																		\
	template <typename type>												\
	vector2<type>& operator oper##=(vector2<type>& v1, vector2<type> v2)	\
	{																		\
		v1.x oper##= v2.x;													\
		v1.y oper##= v2.y;													\
		return v1;															\
	}

#define AGE_SCALAR2_OPERATOR_IMPL(oper)										\
	template <typename type>												\
	vector2<type> operator oper(vector2<type> v1, type value)				\
	{																		\
		return vector2<type>(v1.x oper value, v1.y oper value);				\
	}																		\
	template <typename type>												\
	vector2<type>& operator oper##=(vector2<type>& v1, type value)			\
	{																		\
		v1.x oper##= value;													\
		v1.y oper##= value;													\
		return v1;															\
	}

	AGE_VECTOR2_OPERATOR_IMPL(+);
	AGE_VECTOR2_OPERATOR_IMPL(-);
	AGE_VECTOR2_OPERATOR_IMPL(*);
	AGE_VECTOR2_OPERATOR_IMPL(/);

	AGE_SCALAR2_OPERATOR_IMPL(+);
	AGE_SCALAR2_OPERATOR_IMPL(-);
	AGE_SCALAR2_OPERATOR_IMPL(*);
	AGE_SCALAR2_OPERATOR_IMPL(/);

	using fvec2 = vector2<float>;
	using dvec2 = vector2<double>;
	using vec2 = fvec2;

	using ivec2 = vector2<int>;
	using uivec2 = vector2<unsigned>;
	using uvec2 = uivec2;

	using vec2_size = vector2<size_t>;
}
