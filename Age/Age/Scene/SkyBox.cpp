#include "SkyBox.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"
#include "Age/Object/Mesh.hpp"
#include "Age/egd.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/LL/opengl.h"

namespace a_game_engine
{
	const VertexBuffer* SkyBox::cube = nullptr;

	SkyBox::SkyBox(Scene& scene)
		: scene(&scene)
	{
	}

	void SkyBox::draw(const Camera& camera, const ShaderProgram* s) const
	{
		if (s == nullptr)
			s = shader;

		if (cubemap && cube)
		{
			Pipeline::setDepthFunc(DepthFunc::LEqual);
			s->use();
			mat4 view = camera.transform.getWorld();
			view.data[3][0] = view.data[3][1] = view.data[3][2] = 0.f;
			s->setUniform(s->getLocation("view"), view);
			s->setUniform(s->getLocation("projection"), camera.getProjection());
			s->setUniform(s->getLocation("skybox"), *cubemap, 0);
			cube->draw();
		}
	}
	int SkyBox::getSlot() //TODO: is this really necessary
	{
		static int maxSlots = 16;
		if (maxSlots == 0)
			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxSlots);

		return maxSlots - 1;
	}
}

