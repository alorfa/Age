#include "SkyBox.hpp"
#include "Age/LL/Shader/Shader.hpp"
#include "Mesh3D.hpp"

namespace a_game_engine
{
	const Mesh3D* SkyBox::cube = nullptr;

	SkyBox::SkyBox(Node3D* parent)
		: Node3D(parent)
	{
	}

	void SkyBox::draw(const mat4& parent, const Node3D& sc, const Camera3D& camera, const Shader* s) const
	{
		if (s == nullptr)
			s = shader;

		if (cubemap and cube)
		{
			s->use();
			s->setCamera(camera);
			s->setUniform("skybox", *cubemap, 0);

			mat4 curTransform = parent * transform.getMatrix();
			cube->buffer.draw();
		}

		Node3D::draw(parent, sc, camera, s);
	}
}

