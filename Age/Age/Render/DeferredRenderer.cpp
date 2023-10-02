#include "DeferredRenderer.hpp"
#include "Age/egd.hpp"
#include "Age/Object/Node3D.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/Scene/SkyBox.hpp"
#include "Age/Scene/Scene3D.hpp"

namespace a_game_engine
{
	DeferredRenderer::DeferredRenderer()
	{
		//shader = &egd.shaders.loadPostproc(egd.res / "shader/postproc.pasl");
		gbuffer.textures.resize(3);
	}

	void DeferredRenderer::updateSize(const uvec2& newSize)
	{
		size = newSize;
		ImageInfo baseColorRGB_RoughnessA{ newSize, nullptr, TextureFormat::RGBA_Float16 },
			normalRGB_MetalnessA{ newSize, nullptr, TextureFormat::RGBA },
			posRGB{ newSize, nullptr, TextureFormat::RGB_Float32 };
		gbuffer.createRenderBuffer(newSize);
		gbuffer.textures[0].create({ baseColorRGB_RoughnessA, false });
		gbuffer.textures[1].create({ normalRGB_MetalnessA, false });
		gbuffer.textures[2].create({ posRGB, false });
		gbuffer.textures[0].setFiltering(TextureFiltering::Linear);
		gbuffer.textures[0].setWrap(TextureWrap::ClampToEdge);
		gbuffer.textures[1].setFiltering(TextureFiltering::Linear);
		gbuffer.textures[1].setWrap(TextureWrap::ClampToEdge);
		gbuffer.textures[2].setFiltering(TextureFiltering::Linear);
		gbuffer.textures[2].setWrap(TextureWrap::ClampToEdge);
		gbuffer.create();
	}

	void DeferredRenderer::drawObject(const Node3D& o, const Scene3DInfo& info)
	{
		static mat4 identity;
		o.draw(identity, info);
	}

	void DeferredRenderer::drawScene(const Scene3D& scene, const Camera3D& camera)
	{
		Scene3DInfo info;
		info.camera = &camera;
		info.props.push_back({ "skybox", SkyBox::getSlot() });
		ShaderSettings::Deferred deferred;
		deferred.bindings = { 4, 4, 3 };
		deferred.paintingFuncIndex = 0;
		info.shaderSettings = deferred;

		Pipeline::set3DContext();
		gbuffer.use();
		Pipeline::clear({ 0.1f, 0.1f, 0.1f });
		scene.rootNode->forEachConst([&](const Node3D& n) {
			drawObject(n, info);
			});
		scene.skyBox.draw(camera, nullptr);

		gbuffer.useDefault(size);
		Pipeline::set2DContext();
		auto* verts = &VertexBuffer::getDefFramebuf();
		/*shader->use();
		shader->setUniform(shader->getLocation("baseColorRGB_roughnesA"), gbuffer.textures[0], 0);
		shader->setUniform(shader->getLocation("normalRGB_metalnessA"), gbuffer.textures[1], 1);
		shader->setUniform(shader->getLocation("posRGB"), gbuffer.textures[2], 2);*/
		verts->draw();
	}
}
