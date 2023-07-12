#include "Cube.hpp"
#include "Age/Game/Gdata.hpp"

#include "Age/Auxiliary/print_math.hpp"

namespace a_game
{
	Cube::Cube()
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
	void Cube::draw(const Scene3D& sc, const Camera3D& camera, const Shader* s) const
	{
		if (s == nullptr)
			s = shader;

		s->use();

		Object3D::draw(sc, camera, s);
	}
}