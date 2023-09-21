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
            "#define AGE_RENDERING_MODE_FORWARD\n", s.dirLights, s.pointLights, s.spotLights);
        DetailedSettings settings;
        settings.bindings = { 4 };
        settings.defines = constants;
        settings.commonLib = "common lib\n";
        settings.vertexLib = "vertex lib\n";
        settings.fragmentLib = "fragment lib\n";
        settings.mainFunc = "main function\n";
        auto& shader = _forward[s];
        shader = createProgram(settings, _source);
        return *shader;
    }

    const ShaderProgram& Shader::getDeferred(const DeferredSettings& s) const
    {
        auto it = _deferred.find(s);
        if (it != _deferred.end())
            return *it->second;

        DetailedSettings settings;
        settings.bindings = s.bindings;
        settings.defines = "#define AGE_RENDERING_MODE_DEFERRED\n";
        settings.commonLib = "common lib\n";
        settings.vertexLib = "vertex lib\n";
        settings.fragmentLib = "fragment lib\n";
        settings.mainFunc = "main function\n";
        settings.forcePaintingOver = s.paintingFunc;
        auto& shader = _deferred[s];
        shader = createProgram(settings, _source);
        return *shader;
    }
    std::unique_ptr<ShaderProgram> Shader::createProgram(const DetailedSettings& s, const std::string& source)
    {
        std::string bindings;
        for (uint i = 0; i < (uint)s.bindings.size(); i++)
            bindings += generateBinding(s.bindings[i], i);
        
        const std::string vertex = std::format("#define AGE_VERTEX\n{}{}{}{}", 
            s.defines, s.commonLib, s.vertexLib, source);
        std::string fragment;
        if (s.forcePaintingOver.size() == 0)
        {
            fragment = std::format("#define AGE_FRAGMENT\n{}{}{}{}{}{}",
                s.defines, bindings, s.commonLib, s.fragmentLib, source, s.mainFunc);
        }
        else
        {
            fragment = std::format("#define AGE_FRAGMENT\n#define AGE_LIGHT_MODE_FORCE\n{}{}{}{}{}{}{}",
                s.defines, bindings, s.commonLib, s.fragmentLib, source, s.forcePaintingOver, s.mainFunc);
        }
        auto vsh = ShaderCompiler::loadFromMemory(vertex.c_str(), ShaderCompiler::Vertex);
        auto fsh = ShaderCompiler::loadFromMemory(fragment.c_str(), ShaderCompiler::Fragment);
        auto result = std::make_unique<ShaderProgram>(vsh, fsh);
        if (result->isValid())
            return result;
        return nullptr;
    }
    std::string Shader::generateBinding(uint components, uint number)
    {
        std::string name;
        if (number == 0)
            name = "age_FragColor";
        else
            name = "age_FragColor" + std::to_string(number);

        const std::string type = toVecType(components);
        return std::format("layout(location = {}) out {} {};\n", number, type, name);
    }
    std::string Shader::toVecType(uint vecComponents)
    {
        switch (vecComponents)
        {
        case 1:
            return "float";
        case 2:
            return "vec2";
        case 3:
            return "vec3";
        default:
            return "vec4";
        }
    }
    const ShaderProgram& Shader::getProgram(const Settings& s) const
    {
        if (_lastSettings == s)
            return *_lastShader;

        _lastSettings = s;
        if (s.type == s.Forward)
        {
            const auto& temp = getForward(s.settings.forward);
            if (temp.isValid())
                _lastShader = &temp;
        }
        if (s.type == s.Deferred)
        {
            const auto& temp = getDeferred(s.settings.deferred);
            if (temp.isValid())
                _lastShader = &temp;
        }
        return *_lastShader;
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
    bool Shader::ForwardSettings::operator==(const ForwardSettings& other) const
    {
        return
            dirLights == other.dirLights &&
            pointLights == other.pointLights &&
            spotLights == other.spotLights;
    }
    bool Shader::DeferredSettings::operator<(const DeferredSettings& other) const
    {
        if (bindings.size() < other.bindings.size())
            return true;
        for (uint i = 0; i < (uint)bindings.size(); i++)
        {
            if (bindings[i] < other.bindings[i])
                return true;
        }
        return paintingFunc < other.paintingFunc;
    }

    bool Shader::DeferredSettings::operator==(const DeferredSettings& other) const
    {
        return 
            bindings == other.bindings &&
            paintingFunc == other.paintingFunc;
    }

    Shader::Settings::Settings()
        : type(Settings::Undefined)
    {
    }
    Shader::Settings::Settings(const ForwardSettings& s)
        : type(Settings::Forward)
    {
        settings.forward = s;
    }

    Shader::Settings::Settings(const DeferredSettings& s)
        : type(Settings::Deferred)
    {
        settings.deferred = s;
    }

    bool Shader::Settings::operator==(const Settings& other) const
    {
        if (type != other.type)
            return false;
        if (type == Type::Forward)
            return settings.forward == other.settings.forward;
        if (type == Type::Deferred)
            return settings.deferred == other.settings.deferred;
        return false;
    }
    Shader::Settings& Shader::Settings::operator=(const Settings& s)
    {
        type = s.type;
        if (type == Forward)
            settings.forward = s.settings.forward;
        if (type == Deferred)
            settings.deferred = s.settings.deferred;
        return *this;
    }
}
