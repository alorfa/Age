#pragma once

#include "Mesh3D.hpp"
#include <memory>
#include "Age/Math/mat4.hpp"
#include <string>

namespace a_game_engine
{
	struct Scene3DInfo;

	class Model3D
	{
	public:
		struct Node
		{
			const Model3D* model = nullptr;
			const Node* parent = nullptr;
			std::vector<const Mesh3D*> meshes;
			mat4 transform;
			std::string name;
			std::vector<std::unique_ptr<Node>> children;

			void draw(const mat4& parentTransform, const Scene3DInfo& info, TransformProps& transform) const;
		};

		std::vector<std::unique_ptr<Mesh3D>> meshes;
		std::unique_ptr<Node> rootNode;

		void draw(const mat4& transform, const Scene3DInfo& info) const;
	};
}