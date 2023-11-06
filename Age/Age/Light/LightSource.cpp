#include "LightSource.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/Math/Math.hpp"
#include "Age/Components/LightUpdaters.hpp"
#include "Age/Material/Shader.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Object/Model3D.hpp"

namespace a_game_engine
{
	PointLightSource::PointLightSource(Scene& scene, Node* parent)
		: Object3D(scene, parent, Node::Influencing)
	{
		components.push_back(std::make_unique<PointLightUpdater>(*this));
	}

	SpotLightSource::SpotLightSource(Scene& scene, Node* parent)
		: Object3D(scene, parent, Node::Influencing)
	{
		components.push_back(std::make_unique<SpotLightUpdater>(*this));
	}

	DirLightSource::DirLightSource(Scene& scene, Node* parent)
		: Node(scene, parent, Node::Influencing)
	{}
}