#pragma once

#include "Model3D.hpp"
#include "Age/Transform/Transform3D.hpp"
#include "Age/Transform/Camera3D.hpp"
#include <functional>

namespace a_game_engine
{
	class Object3D
	{
	public:
		const Model3D* model;
		Transform3D transform;
		const Shader* shader;

		void draw(const Scene3D&, const Camera3D&, const Shader*,
			std::function<void(const Object3D&, const Scene3D&, const Camera3D&, const Shader&)>) const;

		virtual void update(float delta);
		virtual void draw(const Scene3D&, const Camera3D&, const Shader* s = nullptr) const;

		virtual ~Object3D() = default;
	};
}
