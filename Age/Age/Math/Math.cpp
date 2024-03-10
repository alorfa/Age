#include "Math.hpp"
#include "mat3.hpp"
#include "mat4.hpp"
#include <glm/mat4x4.hpp>
#include <string.h>
#include <algorithm>
#include "Age/Math/vec3.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace a_game_engine
{
    const float Math::PI = 3.141592f;
    const float Math::TAU = Math::PI * 2.f;

    namespace
    {
        const glm::mat4& asGlm(const mat4& mat)
        {
            return reinterpret_cast<const glm::mat4&>(mat);
        }
        glm::mat4& asGlm(mat4& mat)
        {
            return reinterpret_cast<glm::mat4&>(mat);
        }

        glm::mat3& asGlm(mat3& mat)
        {
            return reinterpret_cast<glm::mat3&>(mat);
        }
        const glm::mat3& asGlm(const mat3& mat)
        {
            return reinterpret_cast<const glm::mat3&>(mat);
        }

        const glm::vec3& asGlm(const vec3& v)
        {
            return reinterpret_cast<const glm::vec3&>(v);
        }
    }

    mat3::mat3()
    {
        memset(this, 0, sizeof(*this));
        data[0] = data[4] = data[8] = 1.f;
    }

    mat3& mat3::operator*=(const mat3& m2)
    {
        asGlm(*this) *= asGlm(m2);
        return *this;
    }

    mat3 mat3::operator*(const mat3& m2)
    {
        mat3 result = *this;
        return result *= m2;
    }

    mat4::mat4()
    {
        reset();
    }
    mat4& mat4::operator*=(const mat4& m2)
    {
        asGlm(*this) *= asGlm(m2);
        return *this;
    }
    mat4 mat4::operator*(const mat4& m2) const
    {
        mat4 result = *this;
        return result *= m2;
    }
    void mat4::reflect()
    {
        std::swap(data[0][1], data[1][0]);
        std::swap(data[0][2], data[2][0]);
        std::swap(data[0][3], data[3][0]);
        std::swap(data[1][2], data[2][1]);
        std::swap(data[1][3], data[3][1]);
        std::swap(data[2][3], data[3][2]);
    }
    void mat4::setPerspective(float fov, float aspectRatio, float near, float far)
    {
        asGlm(*this) = glm::perspective(fov, aspectRatio, near, far);
    }
    void mat4::setOrtho(float viewport, float aspectRatio, float near, float far)
    {
        asGlm(*this) = glm::ortho(
            -viewport * aspectRatio, viewport * aspectRatio, 
            -viewport, viewport, 
            near, far);
    }
    void mat4::setModelMatrix(const vec3& position, const vec3& euler, const vec3& scale)
    {
        reset();
        auto& res = asGlm(*this);
        res = glm::translate(res, asGlm(position));
        res *= glm::eulerAngleXYZ(euler.x, euler.y, euler.z);
        res = glm::scale(res, asGlm(scale));
    }
    void mat4::setViewMatrix(const vec3& position, const vec3& euler)
    {
        reset();
        asGlm(*this) = glm::lookAt(asGlm(position),
            asGlm(position + Math::getForwardDir(euler)), glm::vec3(0, 0, 1));
    }
    void mat4::setViewMatrix(const vec3& offset, const vec3& forward, const vec3& right, const vec3& up)
    {
        data[0][0] = right.x;
        data[0][1] = right.y;
        data[0][2] = right.z;
        data[1][0] = forward.x;
        data[1][1] = forward.y;
        data[1][2] = forward.z;
        data[2][0] = up.x;
        data[2][1] = up.y;
        data[2][2] = up.z;
        data[3][0] = offset.x;
        data[3][1] = offset.y;
        data[3][2] = offset.z;
    }
    void mat4::reset()
    {
        memset(this, 0, sizeof(*this));
        data[0][0] = data[1][1] = data[2][2] = data[3][3] = 1.f;
    }
    float Math::sin(float value)
    {
        return sinf(value);
    }
    float Math::cos(float value)
    {
        return cosf(value);
    }
    float Math::rad(float deg)
    {
        return deg / 180.f * PI;
    }
    float Math::deg(float rad)
    {
        return rad * 180.f / PI;
    }

    vec3 Math::getForwardDir(const vec3& euler)
    {
        float cosx, cosy, sinx, siny;
        cosx = cos(euler.x);
        cosy = cos(euler.y);
        sinx = sin(euler.x);
        siny = sin(euler.y);
        return { -sinx * siny, sinx * cosy, -cosx }; //TODO: z-axis may not be processed correctly
    }
    vec3 Math::getRightDir(const vec3& euler)
    {
        float cosz, cosy, sinz, siny;
        cosz = cos(euler.z);
        cosy = cos(euler.y);
        sinz = sin(euler.z);
        siny = sin(euler.y);
        return { cosz * cosy, siny * cosz, sinz }; //TODO: z-axis may not be processed correctly
    }
    vec3 Math::getUpDir(const vec3& euler)
    {
        float cosx, cosy, cosz, sinx, siny, sinz;
        cosx = cos(euler.x);
        cosy = cos(euler.y);
        cosz = cos(euler.z);
        sinx = sin(euler.x);
        siny = sin(euler.y);
        sinz = sin(euler.z);
        return { -siny * cosx, cosx * cosy, sinx * cosz }; //TODO: z-axis may not be processed correctly
    }
    float Math::saturate(float value)
    {
        return clamp(value, 0.f, 1.f);
    }
}