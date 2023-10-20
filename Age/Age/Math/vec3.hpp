#pragma once

namespace a_game_engine
{
	template <typename T>
	struct vector3
	{
		T x, y, z;

		vector3()
		{
			x = y = z = 0;
		}
		vector3(T value)
		{
			x = y = z = value;
		}
		vector3(T vx, T vy, T vz)
		{
			x = vx;
			y = vy;
			z = vz;
		}

		float square_length() const
		{
			return x * x + y * y + z * z;
		}
		float length() const
		{
			return sqrtf(square_length());
		}

		vector3 new_normalized() const
		{
			return (*this) * (1.f / length());
		}
		void normalize()
		{
			float l = 1.f / length();
			(*this) *= l;
		}

		bool operator==(const vector3& v) const
		{
			return x == v.x && y == v.y && z == v.z;
		}
		bool operator!=(const vector3& v) const
		{
			return !operator==(v);
		}
	};

#define AGE_VECTOR3_OPERATOR_IMPL(oper)										\
	template <typename type>												\
	vector3<type> operator oper(vector3<type> v1, vector3<type> v2)			\
	{																		\
		return vector3<type>(v1.x oper v2.x, v1.y oper v2.y, v1.z oper v2.z);\
	}																		\
	template <typename type>												\
	vector3<type>& operator oper##=(vector3<type>& v1, vector3<type> v2)	\
	{																		\
		v1.x oper##= v2.x;													\
		v1.y oper##= v2.y;													\
		v1.z oper##= v2.z;													\
		return v1;															\
	}

#define AGE_SCALAR3_OPERATOR_IMPL(oper)										\
	template <typename type>												\
	vector3<type> operator oper(vector3<type> v1, type value)				\
	{																		\
		return vector3<type>(v1.x oper value, v1.y oper value, v1.z oper value);\
	}																		\
	template <typename type>												\
	vector3<type>& operator oper##=(vector3<type>& v1, type value)			\
	{																		\
		v1.x oper##= value;													\
		v1.y oper##= value;													\
		v1.z oper##= value;													\
		return v1;															\
	}

	AGE_VECTOR3_OPERATOR_IMPL(+);
	AGE_VECTOR3_OPERATOR_IMPL(-);
	AGE_VECTOR3_OPERATOR_IMPL(*);
	AGE_VECTOR3_OPERATOR_IMPL(/);

	AGE_SCALAR3_OPERATOR_IMPL(+);
	AGE_SCALAR3_OPERATOR_IMPL(-);
	AGE_SCALAR3_OPERATOR_IMPL(*);
	AGE_SCALAR3_OPERATOR_IMPL(/);

	using fvec3 = vector3<float>;
	using dvec3 = vector3<double>;
	using vec3 = fvec3;

	using ivec3 = vector3<int>;
	using uivec3 = vector3<unsigned>;
	using uvec3 = uivec3;

	using vec3_size = vector3<size_t>;
}