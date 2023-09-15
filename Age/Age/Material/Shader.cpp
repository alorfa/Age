#include "Shader.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/LL/Shader/ShaderCompiler.hpp"

namespace a_game_engine
{
    Shader::Shader(const std::string& source)
        : _source(source)
    {
    }

    const ShaderProgram& Shader::getForward(const ForwardSettings& s) const
    {
        auto it = _forward.find(s);
        if (it != _forward.end())
            return *it->second;

        const auto constants = std::format(
            "#define AGE_MAX_DIR_LIGHTS {}\n"
            "#define AGE_MAX_POINT_LIGHTS {}\n"
            "#define AGE_MAX_SPOT_LIGHTS {}\n"
            "#define AGE_FORWARD_SHADING\n", s.dirLights, s.pointLights, s.spotLights);
        const std::string vertex = "#define AGE_VERTEX\n" + constants + _source;
        const std::string fragment = "#define AGE_FRAGMENT\n" + constants + _source;
        auto vsh = ShaderCompiler::loadFromMemory(vertex.c_str(), ShaderCompiler::Vertex);
        auto fsh = ShaderCompiler::loadFromMemory(fragment.c_str(), ShaderCompiler::Fragment);
        auto& shader = _forward[s];
        shader = std::make_unique<ShaderProgram>(vsh, fsh);
        return *shader;
    }

    const ShaderProgram& Shader::getDeferred() const
    {
        if (!_deferred)
        {
            const std::string constants = "#define AGE_DEFERRED_SHADING\n";
            const std::string vertex = "#define AGE_VERTEX\n" + constants + _source;
            const std::string fragment = "#define AGE_FRAGMENT\n" + constants + _source;
            auto vsh = ShaderCompiler::loadFromMemory(vertex.c_str(), ShaderCompiler::Vertex);
            auto fsh = ShaderCompiler::loadFromMemory(fragment.c_str(), ShaderCompiler::Fragment);
            _deferred = std::make_unique<ShaderProgram>(vsh, fsh);
        }
        return *_deferred;
    }
    bool Shader::ForwardSettings::operator<(const ForwardSettings& other) const
    {
        if (dirLights < other.dirLights)
            return true;
        if (pointLights < other.pointLights)
            return true;
        if (spotLights < other.spotLights)
            return true;
        return false;
    }
}

