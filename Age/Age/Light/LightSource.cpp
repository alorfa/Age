#include "LightSource.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/Math/Math.hpp"
#include "Age/Components/LightUpdaters.hpp"
#include "Age/Material/Shader.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Object/Model3D.hpp"

namespace a_game_engine
{
	PointLightSource::PointLightSource(Scene3D& scene, Node3D* parent)
		: Object3D(scene, parent, Node3D::Influencing)
	{
		components.push_back(std::make_unique<PointLightUpdater>(*this));
	}

	SpotLightSource::SpotLightSource(Scene3D& scene, Node3D* parent)
		: Object3D(scene, parent, Node3D::Influencing)
	{
		components.push_back(std::make_unique<SpotLightUpdater>(*this));
	}

	DirLightSource::DirLightSource(Scene3D& scene, Node3D* parent)
		: Node3D(scene, parent, Node3D::Influencing)
	{}
}