#include "Shader.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/LL/Shader/ShaderCompiler.hpp"
#include "Age/Resource/Logger.hpp"
#include "Age/Resource/File.hpp"

namespace a_game_engine
{
    Shader::GlslCode Shader::translateToGlsl(const ShaderSettings::Detailed& s, const std::string& source)
    {
        std::string bindings;
        for (uint i = 0; i < (uint)s.bindings.size(); i++)
            bindings += generateBinding(s.bindings[i], i);

        GlslCode result;
        result.vert = std::format("#define AGE_VERTEX\n{}\n{}\n{}\n{}\n",
            s.defines, s.include->common, s.include->vertex, source);
        std::string fragment;
        if (s.paintingFuncIndex < 0)
        {
            result.frag = std::format("#define AGE_FRAGMENT\n{}\n{}\n{}\n{}\n{}\n{}",
                s.defines, bindings, s.include->common, s.include->fragment, source, s.include->fragMain);
        }
        else
        {
            const std::string fullFunctionCode =
                "void force_paintOver()\n{\n" +
                ShaderSettings::paintingFunctions[s.paintingFuncIndex] + "}";
            result.frag = std::format("#define AGE_FRAGMENT\n#define AGE_LIGHT_MODE_FORCE\n"
                "{}\n{}{}\n{}\n{}\n",
                s.defines, bindings, s.include->common, s.include->fragment, source);
            result.frag += fullFunctionCode;
            result.frag += s.include->fragMain;
        }
        Logger::logInfo(result.vert);
        Logger::logInfo(result.frag);
        File::writeToFile("res/shader/tmp.vsh", result.vert);
        File::writeToFile("res/shader/tmp.fsh", result.frag);
        return result;
    }
    Shader::Shader(const std::string& source)
        : _source(source)
    {
    }

    const ShaderProgram& Shader::getForward(const ShaderSettings::Forward& s) const
    {
        auto it = _forward.find(s);
        if (it != _forward.end())
            return *it->second;

        ShaderSettings::Detailed settings{ShaderSettings::include, s};
        
        auto& shader = _forward[s];
        shader = createProgram(settings, _source);
        return *shader;
    }

    const ShaderProgram& Shader::getDeferred(const ShaderSettings::Deferred& s) const
    {
        auto it = _deferred.find(s);
        if (it != _deferred.end())
            return *it->second;

        ShaderSettings::Detailed settings{ShaderSettings::include, s};
        
        auto& shader = _deferred[s];
        shader = createProgram(settings, _source);
        return *shader;
    }
    std::unique_ptr<ShaderProgram> Shader::createProgram(const ShaderSettings::Detailed& s, const std::string& source)
    {
        auto [vert, frag] = translateToGlsl(s, source);
        auto vsh = ShaderCompiler::loadFromMemory(vert.c_str(), ShaderCompiler::Vertex);
        auto fsh = ShaderCompiler::loadFromMemory(frag.c_str(), ShaderCompiler::Fragment);
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
