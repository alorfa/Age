#include "Cube.hpp"
#include "Age/Game/Gdata.hpp"

#include "Age/Auxiliary/print_math.hpp"

namespace a_game
{
	Cube::Cube(Node3D* parent)
		: Object3D(parent)
	{
		auto& m = gdata->modelLoader.load(gdata->res / "model/daedric/scene.gltf", 
			ModelLoader::Settings{ vec3{5.f}, false, false, true });
		/*m.meshes[0]->material->spec = m.meshes[0]->material->diff;
		m.meshes[0]->material->shininess = 50.f;*/
		model = &m;
		
		shader = &gdata->shaderLoader.load(gdata->res / "shader/pbrNormal");
	}
	void Cube::update(float delta)
	{
		time += delta;
		//transform.changePosition().y += s * delta;
		//transform.changeRotation().z += delta * 0.20f;
	}
}