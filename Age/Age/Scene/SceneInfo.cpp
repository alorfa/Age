#include "SceneInfo.hpp"
#include "Age/Object/Node.hpp"
#include "Age/Components/LightComponents.hpp"
#include <format>
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	void SceneInfo::addLights(const Node& node)
	{
		node.forEachConstLocal([&](const Node& n) {
			for (auto& comp : n.components)
			{
				auto* pointLight = dynamic_cast<const PointLightComponent*>(comp.get());
				if (pointLight)
				{
					props.push_back({ std::format("pointLightSources[{}].color", lights.point), pointLight->getLight().color * pointLight->getLight().attOffset});
					props.push_back({ std::format("pointLightSources[{}].ambient", lights.point), pointLight->getLight().ambient * pointLight->getLight().attOffset });
					props.push_back({ std::format("pointLightSources[{}].pos", lights.point), pointLight->getLight().pos });
					props.push_back({ std::format("pointLightSources[{}].attOffset", lights.point), pointLight->getLight().attOffset });
					props.push_back({ std::format("pointLightSources[{}].maxDist", lights.point), pointLight->getLight().radius });
					props.push_back({ std::format("pointLightSources[{}].attOffset", lights.spot), pointLight->getLight().attOffset });
					props.push_back({ std::format("pointLightSources[{}].maxDist", lights.spot), pointLight->getLight().radius });
					props.push_back({ std::format("pointLightSources[{}].sourceRadius", lights.point), pointLight->getLight().size * 0.5f });
					lights.point++;
					continue;
				}
				auto* spotLight = dynamic_cast<const SpotLightComponent*>(comp.get());
				if (spotLight)
				{
					props.push_back({ std::format("spotLightSources[{}].color", lights.spot), spotLight->getLight().color * spotLight->getLight().attOffset });
					props.push_back({ std::format("spotLightSources[{}].ambient", lights.spot), spotLight->getLight().ambient * spotLight->getLight().attOffset });
					props.push_back({ std::format("spotLightSources[{}].pos", lights.spot), spotLight->getLight().pos });
					props.push_back({ std::format("spotLightSources[{}].attOffset", lights.spot), spotLight->getLight().attOffset });
					props.push_back({ std::format("spotLightSources[{}].maxDist", lights.spot), spotLight->getLight().radius });
					props.push_back({ std::format("spotLightSources[{}].sourceRadius", lights.spot), spotLight->getLight().size * 0.5f });
					props.push_back({ std::format("spotLightSources[{}].dir", lights.spot), spotLight->getLight().dir });
					props.push_back({ std::format("spotLightSources[{}].cutOff", lights.spot), Math::cos(spotLight->getLight().cutOff) });
					props.push_back({ std::format("spotLightSources[{}].outerCutOff", lights.spot), Math::cos(spotLight->getLight().outerCutOff) });
					lights.spot++;
					continue;
				}
				auto* dirLight = dynamic_cast<const DirLightComponent*>(comp.get());
				if (dirLight)
				{
					props.push_back({ std::format("dirLightSources[{}].color", lights.dir), dirLight->light.color });
					props.push_back({ std::format("dirLightSources[{}].ambient", lights.dir), dirLight->light.ambient });
					props.push_back({ std::format("dirLightSources[{}].dir", lights.dir), dirLight->light.dir });
					props.push_back({ std::format("dirLightSources[{}].sourceRadius", lights.dir), dirLight->light.size * 0.5f });
					lights.dir++;
					continue;
				}
				addLights(n);
			}
		});
	}
}

