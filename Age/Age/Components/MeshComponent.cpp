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
	MeshComponent& MeshComponent::setMesh(const ModelResource::Mesh& m)
	{
		mesh.buffer = &m.buffer;
		mesh.material.props = m.props;
		return *this;
	}
	void MeshComponent::draw(const SceneInfo& info) const
	{
		if (not mesh.buffer or
			(info.drawingCondition and not info.drawingCondition(mesh.material)))
			return;
		
		TransformProps transform; //TODO: do need to copy all the data for every mesh?
		transform.projectionView = &info.projView;
		transform.cameraPos = info.cameraPos;
		transform.near = info.nearFar.x;
		transform.far = info.nearFar.y;
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
		auto& root = node.addChild();
		root.changeTransform().UNSAFE_setLocalMatrix(model.transform);
		for (const auto& m : model.meshes)
		{
			root.addComponent(std::make_unique<MeshComponent>(root, m));
		}
		for (const auto& c : model.children)
		{
			addResModelNode(root, *c);
		}
	}
	void MeshComponent::setShader(Node& node, const Shader& s)
	{
		node.forEach([&](Node& n) {
			for (auto& comp : n.components)
			{
				auto ptr = dynamic_cast<MeshComponent*>(comp.get());
				if (ptr)
					ptr->mesh.material.shader = &s;
			}
		});
	}
}
