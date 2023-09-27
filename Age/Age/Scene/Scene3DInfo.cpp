#include "Scene3DInfo.hpp"
#include "Age/Object/Node3D.hpp"
#include "Age/Light/LightSource.hpp"
#include <format>
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	void Scene3DInfo::addLights(const Node3D& node)
	{
		for (const auto& light : node.infChildren)
		{/*
			auto* pointLight = light->as<PointLightSource>();
			if (pointLight)
			{
				props.push_back({ std::format("pointLightSources[{}].color", lights.point), pointLight->light.color });
				props.push_back({ std::format("pointLightSources[{}].ambient", lights.point), pointLight->light.ambient });
				props.push_back({ std::format("pointLightSources[{}].pos", lights.point), pointLight->light.pos });
				props.push_back({ std::format("pointLightSources[{}].constant", lights.point), pointLight->light.constant });
				props.push_back({ std::format("pointLightSources[{}].linear", lights.point), pointLight->light.linear });
				props.push_back({ std::format("pointLightSources[{}].quadratic", lights.point), pointLight->light.quadratic });
				lights.point++;
				continue;
			}
			auto* dirLight = light->as<DirLightSource>();
			if (dirLight)
			{
				props.push_back({ std::format("dirLightSources[{}].color", lights.dir), dirLight->light.color });
				props.push_back({ std::format("dirLightSources[{}].ambient", lights.dir), dirLight->light.ambient });
				props.push_back({ std::format("dirLightSources[{}].dir", lights.dir), dirLight->light.dir });
				lights.dir++;
				continue;
			}*/
			auto* spotLight = light->as<SpotLightSource>();
			if (spotLight)
			{
				props.push_back({ std::format("spotLightSources[{}].color", lights.spot), spotLight->light.color });
				props.push_back({ std::format("spotLightSources[{}].ambient", lights.spot), spotLight->light.ambient});
				props.push_back({ std::format("spotLightSources[{}].pos", lights.spot), spotLight->light.pos });
				props.push_back({ std::format("spotLightSources[{}].constant", lights.spot), spotLight->light.constant });
				props.push_back({ std::format("spotLightSources[{}].linear", lights.spot), spotLight->light.linear });
				props.push_back({ std::format("spotLightSources[{}].quadratic", lights.spot), spotLight->light.quadratic });
				//TODO: ????????? fix this shit
				props.push_back({ std::format("spotLightSources[{}].cutOff", lights.spot), Math::cos(spotLight->light.cutOff) });
				props.push_back({ std::format("spotLightSources[{}].outerCutOff", lights.spot), Math::cos(spotLight->light.outerCutOff) });
				lights.spot++;
				continue;
			}
		}
	}
}

