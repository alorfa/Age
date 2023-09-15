#include "Model3D.hpp"

namespace a_game_engine
{
    void Model3D::draw(const mat4& transform, const ShaderProgram& shader, uint textureOrder) const
    {
        rootNode->draw(transform, shader, textureOrder);
    }

    void Model3D::Node::draw(const mat4& parentTransform, const ShaderProgram& shader, uint textureOrder) const
    {
        mat4 t = parentTransform * transform;
        shader.setUniform(shader.getLocation("model"), t);

        for (const auto& mesh : meshes)
            mesh->draw(shader, textureOrder);

        for (const auto& node : children)
            node->draw(t, shader, textureOrder);
    }
}