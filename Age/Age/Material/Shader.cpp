#include "Shader.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/LL/Shader/ShaderModule.hpp"
#include "Age/Other/Logger.hpp"
#include "Age/Resource/File.hpp"

namespace a_game_engine
{
    Shader::GlslCode Shader::translateToGlsl(const ShaderSettings::Detailed& s, const std::string& source)
    {
        std::string bindings;
        for (uint i = 0; i < (uint)s.bindings.size(); i++)
            bindings += generateBinding(s.bindings[i], i);

        GlslCode result;
        result.vert = std::format("#define AGE_VERTEX\n{}\n{}\n{}\n{}\n{}",
            s.defines, s.include->vertLib, s.include->vertInc, source, s.include->vertMain);
        std::string fragment;
        if (s.paintingFunc == nullptr)
        {
            result.frag = std::format("#define AGE_FRAGMENT\n{}\n{}\n{}\n{}\n{}\n{}",
                s.defines, bindings, s.include->fragLib, s.include->fragInc, source, s.include->fragMain);
        }
        else
        {
            const std::string fullFunctionCode =
                "void force_paintOver()\n{\n" +
                *s.paintingFunc + "}\n";
            result.frag = std::format("#define AGE_FRAGMENT\n#define AGE_LIGHT_MODE_FORCE\n"
                "{}\n{}{}\n{}\n{}\n{}{}",
                s.defines, bindings, s.include->fragLib, s.include->fragInc, source, fullFunctionCode, s.include->fragMain);
        }
        return result;
    }
    Shader::Shader(const std::string& source, const ShaderSettings::Include& include)
        : _source(source), _include(&include)
    {
    }

    Shader::Shader(const std::string& source)
        : _source(source)
    {
        _include = &ShaderSettings::include;
    }

    const ShaderProgram& Shader::getForward(const ShaderSettings::Forward& s) const
    {
        auto it = _forward.find(s);
        if (it != _forward.end())
            return *it->second;

        ShaderSettings::Detailed settings{*_include, s};
        
        auto& shader = _forward[s];
        shader = createProgram(settings, _source);
        return *shader;
    }

    const ShaderProgram& Shader::getDeferred(const ShaderSettings::Deferred& s) const
    {
        auto it = _deferred.find(s);
        if (it != _deferred.end())
            return *it->second;

        ShaderSettings::Detailed settings{*_include, s};
        
        auto& shader = _deferred[s];
        shader = createProgram(settings, _source);
        return *shader;
    }
    std::unique_ptr<ShaderProgram> Shader::createProgram(const ShaderSettings::Detailed& s, const std::string& source)
    {
        auto [vert, frag] = translateToGlsl(s, source);
        ShaderModule vsh{ vert, ShaderModule::Vertex };
        if (not vsh.isValid())
            return nullptr;
        ShaderModule fsh{ frag, ShaderModule::Fragment };
        if (not fsh.isValid())
            return nullptr;
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
    const ShaderProgram& Shader::getProgram(const ShaderSettings::Common& s) const
    {
        if (_lastSettings == s)
            return *_lastShader;

        _lastSettings = s;
        ShaderSettings::Detailed detailed(ShaderSettings::include, s);
        if (s.type == ShaderSettings::Common::Type::Forward)
        {
            const auto& temp = getForward(std::get<ShaderSettings::Forward>(s.settings));
            if (temp.isValid())
                _lastShader = &temp;
        }
        if (s.type == ShaderSettings::Common::Type::Deferred)
        {
            const auto& temp = getDeferred(std::get<ShaderSettings::Deferred>(s.settings));
            if (temp.isValid())
                _lastShader = &temp;
        }

        return *_lastShader;
    }

}
