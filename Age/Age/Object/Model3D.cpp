#include "Model3D.hpp"
#include "Age/Scene/Scene3DInfo.hpp"
#include "Age/Transform/Camera3D.hpp"

namespace a_game_engine
{
    void Model3D::draw(const mat4& t, const Scene3DInfo& info) const
    {
        TransformProps transform;
        transform.view = &info.camera->transform.getWorld();
        transform.proj = &info.camera->getProjection();
        transform.cameraPos = info.camera->transform.getPosition();
        transform.near = info.camera->getNearFar().x;
        transform.far = info.camera->getNearFar().y;
        rootNode->draw(t, info, transform);
    }

    void Model3D::Node::draw(const mat4& parentTransform, const Scene3DInfo& info, TransformProps& tr) const
    {
        mat4 t = parentTransform * transform;

        for (const auto& mesh : meshes)
        {
            tr.model = &t;
            Mesh3D::RenderInfo renderInfo = {tr, info.props, *info.shader, info.shaderSettings};
            if (!info.drawingCondition or info.drawingCondition(mesh->material))
                mesh->draw(renderInfo);
        }

        for (const auto& node : children)
            node->draw(t, info, tr);
    }
}