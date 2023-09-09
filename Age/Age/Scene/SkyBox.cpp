#include "SkyBox.hpp"
#include "Age/LL/Shader/Shader.hpp"
#include "Age/Object/Mesh3D.hpp"
#include "Age/egd.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/LL/opengl.h"

namespace a_game_engine
{
	const Mesh3D* SkyBox::cube = nullptr;

	SkyBox::SkyBox(Scene3D& scene)
		: scene(&scene)
	{
	}

	void SkyBox::draw(const Camera3D& camera, const Shader* s) const
	{
		if (s == nullptr)
			s = shader;

		if (cubemap && cube)
		{
			Pipeline::setDepthFunc(DepthFunc::LEqual);
			s->use();
			mat4 view = camera.transform.getMatrix();
			view.data[3][0] = view.data[3][1] = view.data[3][2] = 0.f;
			s->setUniform("view", view);
			s->setUniform("projection", camera.getProjection());
			s->setUniform("skybox", *cubemap, 0);
			cube->buffer.draw();
		}
	}
	int SkyBox::getSlot()
	{
		static int maxSlots = 16;
		if (maxSlots == 0)
			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxSlots);

		return maxSlots - 1;
	}
}

