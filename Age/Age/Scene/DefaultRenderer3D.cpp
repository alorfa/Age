#pragma once

#include "DefaultRenderer3D.hpp"
#include "Age/Scene/Scene3D.hpp"
#include "Age/Resource/Logger.hpp"
#include "Age/LL/opengl.h"
#include "Age/Resource/ShaderLoader.hpp"

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
		mainFb.create(FrameBuffer2D::Settings(newSize, TextureFormat::RGB, true));
		mainFb.texture.setFiltering(TextureFiltering::Linear);
		mainFb.texture.setWrap(TextureWrap::ClampToEdge);
		Logger::logDebug("Default renderer is resized");
	}
	void DefaultRenderer3D::drawObject(const Object3D& o, const Scene3D& sc,
		const Camera3D& c, const Shader* s) const
	{
		o.draw(sc, c, s);
	}
	void DefaultRenderer3D::drawScene(const Scene3D& sc) const
	{
		/*set3DContext();
		mainFb.use();*/

		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (const auto& obj : sc.objects)
			drawObject(*obj, sc, *sc.activeCamera, nullptr);
		for (const auto& obj : sc.pointLights)
			drawObject(*obj, sc, *sc.activeCamera, nullptr);
		for (const auto& obj : sc.spotLights)
			drawObject(*obj, sc, *sc.activeCamera, nullptr);

		/*mainFb.useDefault(size);
		set2DContext();
		auto* verts = &VertexBuffer::getDefFramebuf();
		shader->use();
		shader->setUniform("tex", mainFb.texture, 0);
		verts->draw();*/
	}
}