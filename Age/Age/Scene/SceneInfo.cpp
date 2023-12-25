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
					props.push_back({ std::format("pointLightSources[{}].color", lights.point), pointLight->getLight().color});
					props.push_back({ std::format("pointLightSources[{}].ambient", lights.point), pointLight->getLight().ambient });
					props.push_back({ std::format("pointLightSources[{}].pos", lights.point), pointLight->getLight().pos });
					props.push_back({ std::format("pointLightSources[{}].constant", lights.point), pointLight->getLight().constant });
					props.push_back({ std::format("pointLightSources[{}].linear", lights.point), pointLight->getLight().linear });
					props.push_back({ std::format("pointLightSources[{}].quadratic", lights.point), pointLight->getLight().quadratic });
					props.push_back({ std::format("pointLightSources[{}].radius", lights.point), pointLight->getLight().radius });
					lights.point++;
					continue;
				}
				auto* spotLight = dynamic_cast<const SpotLightComponent*>(comp.get());
				if (spotLight)
				{
					props.push_back({ std::format("spotLightSources[{}].color", lights.spot), spotLight->getLight().color });
					props.push_back({ std::format("spotLightSources[{}].ambient", lights.spot), spotLight->getLight().ambient });
					props.push_back({ std::format("spotLightSources[{}].pos", lights.spot), spotLight->getLight().pos });
					props.push_back({ std::format("spotLightSources[{}].constant", lights.spot), spotLight->getLight().constant });
					props.push_back({ std::format("spotLightSources[{}].linear", lights.spot), spotLight->getLight().linear });
					props.push_back({ std::format("spotLightSources[{}].quadratic", lights.spot), spotLight->getLight().quadratic });
					props.push_back({ std::format("spotLightSources[{}].dir", lights.spot), spotLight->getLight().dir });
					props.push_back({ std::format("spotLightSources[{}].cutOff", lights.spot), Math::cos(spotLight->getLight().cutOff) });
					props.push_back({ std::format("spotLightSources[{}].outerCutOff", lights.spot), Math::cos(spotLight->getLight().outerCutOff) });
					props.push_back({ std::format("spotLightSources[{}].radius", lights.spot), spotLight->getLight().radius });
					lights.spot++;
					continue;
				}
				auto* dirLight = dynamic_cast<const DirLightComponent*>(comp.get());
				if (dirLight)
				{
					props.push_back({ std::format("dirLightSources[{}].color", lights.dir), dirLight->light.color });
					props.push_back({ std::format("dirLightSources[{}].ambient", lights.dir), dirLight->light.ambient });
					props.push_back({ std::format("dirLightSources[{}].dir", lights.dir), dirLight->light.dir });
					props.push_back({ std::format("dirLightSources[{}].radius", lights.dir), dirLight->light.radius });
					lights.dir++;
					continue;
				}
				addLights(n);
			}
		});
	}
}

