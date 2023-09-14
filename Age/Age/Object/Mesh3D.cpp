#include "Age/Object/Mesh3D.hpp"

namespace a_game_engine
{
	void Mesh3D::draw(const ShaderProgram& s, uint textureOrder) const
	{
		uint texturesCount = s.setUniform("textures", material, textureOrder);
		s.setUniform("texturesCount", (int)texturesCount);

		buffer.draw();
	}
}
