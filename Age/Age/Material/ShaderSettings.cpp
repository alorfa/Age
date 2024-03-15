#include "ShaderSettings.hpp"
#include <format>

namespace a_game_engine
{
    ShaderSettings::Include ShaderSettings::include;
    ShaderSettings::Include ShaderSettings::rawInclude;
    std::vector<ShaderSettings::DeferredImpl> ShaderSettings::deferredImpls;
    std::vector<std::string> ShaderSettings::additionalDefines;
    std::string ShaderSettings::postprocVsh;

    bool ShaderSettings::Forward::operator<(const Forward& other) const
    {
        if (dirLights < other.dirLights)
            return true;
        if (pointLights < other.pointLights)
            return true;
        if (spotLights < other.spotLights)
            return true;
        if (dirLightsShadow < other.dirLightsShadow)
            return true;
        if (definesIndex < other.definesIndex)
            return true;
        return false;
    }
    bool ShaderSettings::Forward::operator==(const Forward& other) const
    {
        return
            dirLights == other.dirLights &&
            pointLights == other.pointLights &&
            spotLights == other.spotLights &&
            dirLightsShadow == other.dirLightsShadow &&
            definesIndex == other.definesIndex;
    }
    bool ShaderSettings::Deferred::operator<(const Deferred& other) const
    {
        return implIndex < other.implIndex && definesIndex < other.definesIndex;
    }

    bool ShaderSettings::Deferred::operator==(const Deferred& other) const
    {
        return implIndex == other.implIndex && definesIndex == other.definesIndex;
    }

    ShaderSettings::Common::Common()
        : type(Common::Type::Undefined)
    {
    }
    ShaderSettings::Common::Common(const Forward& s)
        : type(Common::Type::Forward)
    {
        settings = s;
    }

    ShaderSettings::Common::Common(const Deferred& s)
        : type(Common::Type::Deferred)
    {
        settings = s;
    }

    bool ShaderSettings::Common::operator==(const Common& other) const
    {
        if (type != other.type)
            return false;
        if (type == Type::Forward)
            return std::get<Forward>(settings) == std::get<Forward>(other.settings);
        if (type == Type::Deferred)
            return std::get<Deferred>(settings) == std::get<Deferred>(other.settings);
        return false;
    }
    ShaderSettings::Common& ShaderSettings::Common::operator=(const Common& s)
    {
        type = s.type;
        if (type == Type::Forward)
            settings = s.settings;
        if (type == Type::Deferred)
            settings = s.settings;
        return *this;
    }
    ShaderSettings::Detailed::Detailed(const Include& inc)
    {
        include = &inc;
    }
    ShaderSettings::Detailed::Detailed(const Include& inc, const Forward& f)
    {
        include = &inc;
        create(f);
    }
    ShaderSettings::Detailed::Detailed(const Include& inc, const Deferred& d)
    {
        include = &inc;
        create(d);
    }
    ShaderSettings::Detailed::Detailed(const Include& inc, const Common& c)
    {
        include = &inc;
        create(c);
    }
    void ShaderSettings::Detailed::create(const Forward& f)
    {
        defines = std::format(
            "#define AGE_MAX_DIR_LIGHTS {}\n"
            "#define AGE_MAX_POINT_LIGHTS {}\n"
            "#define AGE_MAX_SPOT_LIGHTS {}\n"
            "#define AGE_MAX_SHADOW_DIR_LIGHTS {}\n"
            "#define AGE_RENDERING_MODE_FORWARD\n", f.dirLights, f.pointLights, f.spotLights, f.dirLightsShadow);
        if (f.definesIndex >= 0)
            defines += ShaderSettings::additionalDefines[f.definesIndex];
        if (f.implIndex >= 0)
        {
            bindings = deferredImpls[f.implIndex].bindings;
            paintingFunc = &deferredImpls[f.implIndex].paintingFunc;
        }
        else
        {
            bindings = { 4 };
        }
    }
    void ShaderSettings::Detailed::create(const Deferred& d)
    {
        bindings = deferredImpls[d.implIndex].bindings;
        defines = "#define AGE_RENDERING_MODE_DEFERRED\n";
        paintingFunc = &deferredImpls[d.implIndex].paintingFunc;
        if (d.definesIndex >= 0)
            defines += ShaderSettings::additionalDefines[d.definesIndex];
    }
    void ShaderSettings::Detailed::create(const Common& f)
    {
        if (f.type == ShaderSettings::Common::Type::Forward)
        {
            create(std::get<Forward>(f.settings));
        }
        if (f.type == ShaderSettings::Common::Type::Deferred)
        {
            create(std::get<Deferred>(f.settings));
        }
    }
}
