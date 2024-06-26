#include "SceneInfo.hpp"
#include "Age/Object/Node.hpp"
#include "Age/Components/LightComponents.hpp"
#include <format>
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	void SceneInfo::addLights(const Node& node, int useShadows)
	{
		node.forEachConstLocal([&](const Node& n) {
			for (auto& comp : n.components)
			{
				auto* pointLight = dynamic_cast<const PointLightComponent*>(comp.get());
				if (pointLight)
				{
					const auto& l = pointLight->getLight();
					props.push_back({ std::format("pointLightSources[{}].color", lights.point), l.color * l.attOffset});
					props.push_back({ std::format("pointLightSources[{}].ambient", lights.point), l.ambient * l.attOffset });
					props.push_back({ std::format("pointLightSources[{}].pos", lights.point), l.pos });
					props.push_back({ std::format("pointLightSources[{}].attOffset", lights.point), l.attOffset });
					props.push_back({ std::format("pointLightSources[{}].maxDist", lights.point), l.radius });
					props.push_back({ std::format("pointLightSources[{}].attOffset", lights.spot), l.attOffset });
					props.push_back({ std::format("pointLightSources[{}].maxDist", lights.spot), l.radius });
					props.push_back({ std::format("pointLightSources[{}].sourceRadius", lights.point), l.size * 0.5f });
					lights.point++;
					continue;
				}
				auto* spotLight = dynamic_cast<const SpotLightComponent*>(comp.get());
				if (spotLight)
				{
					const auto& l = spotLight->getLight();
					props.push_back({ std::format("spotLightSources[{}].color", lights.spot), l.color * l.attOffset });
					props.push_back({ std::format("spotLightSources[{}].ambient", lights.spot), l.ambient * l.attOffset });
					props.push_back({ std::format("spotLightSources[{}].pos", lights.spot), l.pos });
					props.push_back({ std::format("spotLightSources[{}].attOffset", lights.spot), l.attOffset });
					props.push_back({ std::format("spotLightSources[{}].maxDist", lights.spot), l.radius });
					props.push_back({ std::format("spotLightSources[{}].sourceRadius", lights.spot), l.size * 0.5f });
					props.push_back({ std::format("spotLightSources[{}].dir", lights.spot), l.dir });
					props.push_back({ std::format("spotLightSources[{}].cutOff", lights.spot), Math::cos(l.cutOff) });
					props.push_back({ std::format("spotLightSources[{}].outerCutOff", lights.spot), Math::cos(l.outerCutOff) });
					lights.spot++;
					continue;
				}
				auto* dirLight = dynamic_cast<const DirLightComponent*>(comp.get());
				if (dirLight)
				{
					const auto& l = dirLight->getLight();

					bool useCurrentShadow = l.shadowMap.isValid();
					if (useShadows == 0)
						useCurrentShadow = useCurrentShadow && l.useShadow;
					if (useShadows < 0)
						useCurrentShadow = false;
					if (useCurrentShadow)
					{
						dirLight->drawSceneFromShadow(node);
						props.push_back({ std::format("shadowDirLightSources[{}].color", lights.shadowDir), l.color });
						props.push_back({ std::format("shadowDirLightSources[{}].ambient", lights.shadowDir), l.ambient });
						props.push_back({ std::format("shadowDirLightSources[{}].dir", lights.shadowDir), l.dir });
						props.push_back({ std::format("shadowDirLightSources[{}].sourceRadius", lights.shadowDir), l.size * 0.5f });
						props.push_back({ std::format("shadowDirLightSources[{}].bias", lights.shadowDir), dirLight->getBias()});
						props.push_back({ std::format("shadowDirLightSources[{}].texelSize", lights.shadowDir), 
							vec2{1.f / l.shadowMap.getSize().x, 1.f / l.shadowMap.getSize().y} });
						props.push_back({ std::format("shadowVP[{}]", lights.shadowDir), l.viewProj });
						MaterialProperty matprop = { std::format("shadowDirLightSources[{}].shadowMap", lights.shadowDir),
							ShaderProperty::Texture2DProp{ l.shadowMap, (int)lights.shadowDir + 13 } };
						props.push_back(matprop);
						lights.shadowDir++;
					}
					else
					{
						props.push_back({ std::format("dirLightSources[{}].color", lights.dir), l.color });
						props.push_back({ std::format("dirLightSources[{}].ambient", lights.dir), l.ambient });
						props.push_back({ std::format("dirLightSources[{}].dir", lights.dir), l.dir });
						props.push_back({ std::format("dirLightSources[{}].sourceRadius", lights.dir), l.size * 0.5f });
						lights.dir++;
					}
					continue;
				}
				addLights(n);
			}
		});
	}
	void SceneInfo::setCamera(const Camera& c)
	{
		projView = c.getProjection() * c.transform.getWorld();
		cameraPos = c.transform.getPosition();
		nearFar = c.getNearFar();
	}
}

