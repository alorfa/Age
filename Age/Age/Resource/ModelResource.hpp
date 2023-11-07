#pragma once

#include <string>
#include <memory>
#include "Age/Math/mat4.hpp"
#include "Age/LL/Buffers/VertexBuffer.hpp"
#include "Age/Material/Material.hpp"

namespace a_game_engine
{
	class ModelResource
	{
	public:
		class Mesh
		{
		public:
			VertexBuffer buffer;
			MaterialProps props;
		};
		struct Node
		{
			const ModelResource* ownerModel = nullptr;
			const Node* parent = nullptr;
			std::vector<const Mesh*> meshes;
			mat4 transform;
			std::string name;
			std::vector<std::unique_ptr<Node>> children;
		};

		std::vector<std::unique_ptr<Mesh>> meshes;
		std::unique_ptr<Node> rootNode;
	};
}
