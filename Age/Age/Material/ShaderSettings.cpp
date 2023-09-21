#include "ShaderSettings.hpp"
#include <format>

namespace a_game_engine
{
    ShaderSettings::Include ShaderSettings::include;

    bool ShaderSettings::Forward::operator<(const Forward& other) const
    {
        if (dirLights < other.dirLights)
            return true;
        if (pointLights < other.pointLights)
            return true;
        if (spotLights < other.spotLights)
            return true;
        return false;
    }
    bool ShaderSettings::Forward::operator==(const Forward& other) const
    {
        return
            dirLights == other.dirLights &&
            pointLights == other.pointLights &&
            spotLights == other.spotLights;
    }
    bool ShaderSettings::Deferred::operator<(const Deferred& other) const
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

    bool ShaderSettings::Deferred::operator==(const Deferred& other) const
    {
        return
            bindings == other.bindings &&
            paintingFunc == other.paintingFunc;
    }

    ShaderSettings::Common::Common()
        : type(Common::Type::Undefined)
    {
    }
    ShaderSettings::Common::Common(const Forward& s)
        : type(Common::Type::Forward)
    {
        settings.forward = s;
    }

    ShaderSettings::Common::Common(const Deferred& s)
        : type(Common::Type::Deferred)
    {
        settings.deferred = s;
    }

    bool ShaderSettings::Common::operator==(const Common& other) const
    {
        if (type != other.type)
            return false;
        if (type == Type::Forward)
            return settings.forward == other.settings.forward;
        if (type == Type::Deferred)
            return settings.deferred == other.settings.deferred;
        return false;
    }
    ShaderSettings::Common& ShaderSettings::Common::operator=(const Common& s)
    {
        type = s.type;
        if (type == Type::Forward)
            settings.forward = s.settings.forward;
        if (type == Type::Deferred)
            settings.deferred = s.settings.deferred;
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
        bindings = { 4 };
        defines = std::format(
            "#define AGE_MAX_DIR_LIGHTS {}\n"
            "#define AGE_MAX_POINT_LIGHTS {}\n"
            "#define AGE_MAX_SPOT_LIGHTS {}\n"
            "#define AGE_RENDERING_MODE_FORWARD\n", f.dirLights, f.pointLights, f.spotLights);
    }
    void ShaderSettings::Detailed::create(const Deferred& d)
    {
        bindings = d.bindings;
        defines = "#define AGE_RENDERING_MODE_DEFERRED\n";
        forcePaintingOver = d.paintingFunc;
    }
    void ShaderSettings::Detailed::create(const Common& f)
    {
        if (f.type == ShaderSettings::Common::Type::Forward)
        {
            create(f.settings.forward);
        }
        if (f.type == ShaderSettings::Common::Type::Deferred)
        {
            create(f.settings.deferred);
        }
    }
}
