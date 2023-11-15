#pragma once

#include "Age/Resource/ModelResource.hpp"
#include "Age/Object/Mesh.hpp"
#include "Age/Object/Component.hpp"

namespace a_game_engine
{
	class Node;

	class MeshComponent : public Component
	{
	public:
		Mesh mesh;
		const Node* node = nullptr;
		MeshComponent(const Node& owner, const ModelResource::Mesh* mesh = nullptr);

		MeshComponent& setMesh(const ModelResource::Mesh& mesh);

		void draw(const SceneInfo& info) const override;

		static void addModel(Node& node, const ModelResource& model);
		static void addResModelNode(Node& node, const ModelResource::Node& model);
		static void setShader(Node& node, const Shader& s);
	};
}
