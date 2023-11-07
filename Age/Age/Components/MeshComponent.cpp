#include "MeshComponent.hpp"
#include "Age/Transform/Camera.hpp"
#include "Age/Object/Node.hpp"

namespace a_game_engine
{
	MeshComponent::MeshComponent(const Node& owner, const ModelResource::Mesh* m)
		: node(&owner)
	{
		if (m)
		{
			mesh.buffer = &m->buffer;
			mesh.material.props = m->props;
		}
	}
	void MeshComponent::draw(const SceneInfo& info) const
	{
		if (not mesh.buffer or
			(info.drawingCondition and not info.drawingCondition(mesh.material)))
			return;
		
		TransformProps transform; //TODO: do need to copy all the data for every mesh?
		transform.view = &info.camera->transform.getWorld();
		transform.proj = &info.camera->getProjection(); //TODO: combine view and projection matrices
		transform.cameraPos = info.camera->transform.getPosition();
		transform.near = info.camera->getNearFar().x;
		transform.far = info.camera->getNearFar().y;
		transform.model = &node->getTransform().getWorld(); //TODO: and maybe model too...
		Mesh::RenderInfo meshInfo{transform, info.props, *info.shader, info.shaderSettings};
		mesh.draw(meshInfo);
	}

	void MeshComponent::addModel(Node& node, const ModelResource& model)
	{
		addResModelNode(node, *model.rootNode);
	}
	void MeshComponent::addResModelNode(Node& node, const ModelResource::Node& model)
	{
		auto root = std::make_unique<Node>(*node.scene, &node);
		root->changeTransform().UNSAFE_setLocalMatrix(model.transform);
		for (const auto& m : model.meshes)
		{
			root->addComponent(std::make_unique<MeshComponent>(*root, m));
		}
		for (const auto& c : model.children)
		{
			addResModelNode(*root, *c);
		}
		node.addChild(std::move(root));
	}
}
