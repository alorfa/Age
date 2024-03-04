#include "BloomEffect.hpp"
#include "Age/egd.hpp"

namespace a_game_engine
{
	void BloomEffect::destroy()
	{
		textures.clear();
	}
	void BloomEffect::create(uvec2 mip0size, uint mipsCount)
	{
		destroy();
		textures.resize(mipsCount);

		downscale = &egd.shaders.loadPostproc(egd.res / "shader/bloomDownscale.pasl");
		upscale = &egd.shaders.loadPostproc(egd.res / "shader/bloomUpscale.pasl");

		Sampler2DInfo sampler = { TextureFiltering::Linear, TextureWrap::ClampToEdge };
		ImageInfo img;
		img.size = mip0size;
		for (uint i = 0; i < mipsCount; i++)
		{
			textures[i].create(Texture2D::Settings{ img, TextureFormat::RGB_11_11_10, sampler, 1 });
			img.size /= 2u;
		}
	}
	void BloomEffect::use(const Texture& tex)
	{
		const Texture2D* currentTexture = &tex;
		downscale->use();

		for (uint i = 1; i < textures.size(); i++) //i is index of destination texture
		{
			if (i == 1) //instead of 0 texture there should be an HDR buffer
			{
				currentTexture = &tex;
			}
			else
			{
				currentTexture = &textures[i - 1];
			}
			const vec2 texelSize = { 1.f / currentTexture->getSize().x, 1.f / currentTexture->getSize().y };
			downscale->setUniform(downscale->getLocation("tex"), *currentTexture, 0);
			downscale->setUniform(downscale->getLocation("texelSize"), texelSize);
			fb.setTexture(0, textures[i]);
			fb.use();
			VertexBuffer::getDefFramebuf().draw();
		}
		upscale->use();
		for (int i = (int)textures.size() - 2; i >= 0; i--)
		{
			currentTexture = &textures[i + 1];
			const vec2 filterRadius = vec2{ 1.f / currentTexture->getSize().x, 1.f / currentTexture->getSize().y } * radius;
			upscale->setUniform(upscale->getLocation("tex"), *currentTexture, 0);
			upscale->setUniform(upscale->getLocation("radius"), filterRadius);
			fb.setTexture(0, textures[i]);
			fb.use();
			VertexBuffer::getDefFramebuf().draw();
		}
	}
}
