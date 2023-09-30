#include "Age/Object/Mesh3D.hpp"

namespace a_game_engine
{
	void Mesh3D::draw(const ShaderProgram& s) const
	{
		for (const auto& prop : material.props)
			s.setUniform(prop);

		buffer.draw();
	}
	Mesh3D::RenderInfo::RenderInfo(const TransformProps& t, const SceneProps& e)
		: transform(&t),external(&e)
	{}
}
