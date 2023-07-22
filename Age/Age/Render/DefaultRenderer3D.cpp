#pragma once

#include "DefaultRenderer3D.hpp"
#include "Age/Resource/Logger.hpp"
#include "Age/LL/opengl.h"
#include "Age/Resource/ShaderLoader.hpp"
#include "Age/LL/Buffers/VertexBuffer.hpp"
#include "Age/Object/Object3D.hpp"

namespace a_game_engine
{
	namespace
	{
		std::unique_ptr<ShaderLoader> loader = nullptr;
	}
	DefaultRenderer3D::DefaultRenderer3D()
	{
		if (loader == nullptr)
		{
			loader = std::make_unique<ShaderLoader>();
		}
		shader = &loader->load("res/shader/framebuffer");
	}
	void DefaultRenderer3D::updateSize(const uvec2& newSize)
	{
		size = newSize;
		mainFb.create(FrameBuffer2D::Settings(newSize, TextureFormat::RGB_Float16));
		mainFb.texture.setFiltering(TextureFiltering::Linear);
		mainFb.texture.setWrap(TextureWrap::ClampToEdge);
	}
	void DefaultRenderer3D::drawObject(const Node3D& o, const Node3D& sc,
		const Camera3D& c, const Shader* s) const
	{
		o.drawNode(sc.transform.getMatrix(), sc, c, s);
	}
	void DefaultRenderer3D::drawScene(const Node3D& sc, const Camera3D& camera) const
	{
		set3DContext();
		mainFb.use();

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sc.forEachConst([&](const Node3D& n) {
			drawObject(n, sc, camera, nullptr); 
			});

		mainFb.useDefault(size);
		set2DContext();
		auto* verts = &VertexBuffer::getDefFramebuf();
		shader->use();
		shader->setUniform("tex", mainFb.texture, 0);
		verts->draw();
	}
}