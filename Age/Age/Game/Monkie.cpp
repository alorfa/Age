#include "Monkie.hpp"
#include "Age/Game/Gdata.hpp"
#include "Age/Auxiliary/print_math.hpp"

namespace a_game
{
	Monkie::Monkie()
	{
		auto* mod = &gdata->modelLoader.load(gdata->res / "model/kirara/scene.gltf", 
			ModelLoader::Settings{ vec3{10.f}, false});
		model = mod;
		voidTex = &gdata->textureLoader.load(gdata->res / "img/void.png");
		shader = &gdata->shaderLoader.load(gdata->res / "shader/default");
	}
	void Monkie::update(float delta)
	{
		//DEBUG(transform.getMatrix());
		//transform.changeRotation().z += delta;
	}
	void Monkie::draw(const Scene3D& sc, const Camera3D& camera, const Shader* s) const
	{
		if (s == nullptr)
			s = shader;

		s->use();
		s->setUniform("material.specular", *voidTex, 1);
		s->setUniform("material.shininess", 1.f);

		Object3D::draw(sc, camera, s);
	}
}

