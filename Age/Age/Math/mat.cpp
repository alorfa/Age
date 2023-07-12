#include "mat3.hpp"
#include "mat4.hpp"
#include <glm/mat4x4.hpp>
#include <string.h>
#include <algorithm>
#include "Age/Math/vec3.hpp"
#include "Age/Transform/Transform3D.hpp"

namespace a_game_engine
{
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
        memset(this, 0, sizeof(*this));
        data[0] = data[5] = data[10] = data[15] = 1.f;
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
        std::swap(data[1], data[4]);
        std::swap(data[2], data[8]);
        std::swap(data[3], data[12]);
        std::swap(data[6], data[9]);
        std::swap(data[7], data[13]);
        std::swap(data[11], data[14]);
    }
}