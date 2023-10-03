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
		debugPass = &egd.shaders.loadRaw(egd.res / "shader/deferredDebugger.rasl");
		postprocPass = &egd.shaders.loadPostproc(egd.res / "shader/postproc.pasl");
		gbuffer.textures.resize(3);
		screenFb.textures.resize(1);
	}

	void DeferredRenderer::updateSize(const uvec2& newSize)
	{
		size = newSize;
		ImageInfo baseColorRGB_RoughnessA{ newSize, nullptr, TextureFormat::RGBA_Float16 },
			normalRGB_MetalnessA{ newSize, nullptr, TextureFormat::RGBA },
			posRGB{ newSize, nullptr, TextureFormat::RGB_Float32 },
			screenRGB{ newSize, nullptr, TextureFormat::RGB };
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

		screenFb.textures[0].create({ screenRGB , false });
		screenFb.textures[0].setFiltering(TextureFiltering::Near);
		screenFb.textures[0].setWrap(TextureWrap::ClampToEdge);
		screenFb.create();
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
		//info.props.push_back({ "skybox", SkyBox::getSlot() });
		ShaderSettings::Deferred deferred;
		deferred.bindings = { 4, 4, 3 };
		deferred.paintingFuncIndex = 0;
		info.shaderSettings = deferred;

		Pipeline::set3DContext();
		gbuffer.use();
		Pipeline::clear({ 0.f, 0.f, 0.f });
		scene.rootNode->forEachConst([&](const Node3D& n) {
			drawObject(n, info);
			});

		//screenFb.use();
		screenFb.useDefault(size);
		Pipeline::set2DContext();
		auto* rectangleVerts = &VertexBuffer::getDefFramebuf();

		debugPass->use();
		debugPass->setUniform(debugPass->getLocation("offset"), { -0.5f, -0.5f });
		debugPass->setUniform(debugPass->getLocation("scale"), { 0.5f, 0.5f });
		debugPass->setUniform(debugPass->getLocation("tex"), gbuffer.textures[0], 0);
		rectangleVerts->draw();
		debugPass->setUniform(debugPass->getLocation("offset"), { 0.5f, -0.5f });
		debugPass->setUniform(debugPass->getLocation("tex"), gbuffer.textures[1], 1);
		rectangleVerts->draw();

		screenFb.useDefault(size);

		//scene.skyBox.draw(camera, nullptr);
	}
}
