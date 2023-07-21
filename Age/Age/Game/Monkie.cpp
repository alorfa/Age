#include "Monkie.hpp"
#include "Age/Game/Gdata.hpp"
#include "Age/Auxiliary/print_math.hpp"

namespace a_game
{
	Monkie::Monkie(Node3D* parent)
		: Object3D(parent)
	{
		auto* mod = &gdata->modelLoader.load(gdata->res / "model/kirara/scene.gltf", 
			ModelLoader::Settings{ vec3{10.f}, false});
		model = mod;
		shader = &gdata->shaderLoader.load(gdata->res / "shader/default");
	}
	void Monkie::update(float delta)
	{
		//DEBUG(transform.getMatrix());
		//transform.changeRotation().z += delta;
	}
}

