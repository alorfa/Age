#pragma once

#include "Age/Material/ShaderProps.hpp"

namespace a_game_engine
{
	class Camera3D;
	class ShaderProgram;

	struct Scene3DInfo
	{
		const Camera3D* camera = nullptr;
		const ShaderProgram* shader = nullptr;
		SceneProps props;
	};
}
