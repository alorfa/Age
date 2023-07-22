#include "Renderer3D.hpp"
#include "Age/LL/opengl.h"


namespace a_game_engine
{
	void Renderer3D::set3DContext()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer3D::set2DContext()
	{
		glDisable(GL_DEPTH_TEST);
	}
}