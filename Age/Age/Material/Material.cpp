#include "Material.hpp"

namespace a_game_engine
{
	void Material::setValue(const std::string& name, const ShaderProperty& prop)
	{
		for (auto& pr : props)
		{
			if (pr.name == name)
			{
				pr.property = prop;
				return;
			}
		}
		props.push_back({ name, prop });
	}
}

