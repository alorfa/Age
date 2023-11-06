#pragma once

#include "Mesh.hpp"
#include <memory>
#include "Age/Math/mat4.hpp"
#include <string>

namespace a_game_engine
{
	struct SceneInfo;

	class Model
	{
	public:
		struct Node
		{
			const Model* model = nullptr;
			const Node* parent = nullptr;
			std::vector<const Mesh*> meshes;
			mat4 transform;
			std::string name;
			std::vector<std::unique_ptr<Node>> children;

			void draw(const mat4& parentTransform, const SceneInfo& info, TransformProps& transform) const;
		};

		std::vector<std::unique_ptr<Mesh>> meshes;
		std::unique_ptr<Node> rootNode;

		void draw(const mat4& transform, const SceneInfo& info) const;
	};
}