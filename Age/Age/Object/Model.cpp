#include "Model.hpp"
#include "Age/Scene/SceneInfo.hpp"
#include "Age/Transform/Camera.hpp"

namespace a_game_engine
{
    void Model::draw(const mat4& t, const SceneInfo& info) const
    {
        TransformProps transform;
        transform.view = &info.camera->transform.getWorld();
        transform.proj = &info.camera->getProjection();
        transform.cameraPos = info.camera->transform.getPosition();
        transform.near = info.camera->getNearFar().x;
        transform.far = info.camera->getNearFar().y;
        rootNode->draw(t, info, transform);
    }

    void Model::Node::draw(const mat4& parentTransform, const SceneInfo& info, TransformProps& tr) const
    {
        mat4 t = parentTransform * transform;

        for (const auto& mesh : meshes)
        {
            tr.model = &t;
            Mesh::RenderInfo renderInfo = {tr, info.props, *info.shader, info.shaderSettings};
            if (!info.drawingCondition or info.drawingCondition(mesh->material))
                mesh->draw(renderInfo);
        }

        for (const auto& node : children)
            node->draw(t, info, tr);
    }
}