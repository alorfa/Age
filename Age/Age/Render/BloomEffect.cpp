#include "BloomEffect.hpp"
#include "Age/egd.hpp"
#include "Age/Math/Math.hpp"
#include "Age/LL/Pipeline.hpp"

namespace a_game_engine
{
	void BloomEffect::destroy()
	{
		textures.clear();
	}
	void BloomEffect::create(uvec2 mip0size, int mipsCount)
	{
		destroy();
		const int maxMipLevel = TexEnums::getLastMipLevel(mip0size);
		const int curMipLevel = mipsCount <= 0 ? maxMipLevel : Math::clamp(mipsCount - 1, 0, maxMipLevel);
		textures.resize(curMipLevel + 1);

		downscale = &egd.shaders.loadPostproc(egd.res / "shader/bloomDownscale.pasl");
		upscale = &egd.shaders.loadPostproc(egd.res / "shader/bloomUpscale.pasl");

		Sampler2DInfo sampler = { TextureFiltering::Linear, TextureWrap::ClampToEdge };
		ImageInfo img;
		img.size = mip0size;
		for (int i = 0; i <= curMipLevel; i++)
		{
			textures[i].create(Texture2D::Settings{ img, TextureFormat::RGB_11_11_10, sampler, 1 });
			img.size /= 2u;
		}
	}
	void BloomEffect::useDownscale(const Texture& tex, int dstMip, int lastMipLevel)
	{
		Pipeline::setBlendMode(BlendMode::Lerp);

		const Texture2D* currentTexture = &tex;
		lastMipLevel = lastMipLevel <= 0 ? 
			(int)(textures.size() - 1) :
			Math::clamp(lastMipLevel, 1, (int)(textures.size() - 1));
		downscale->use();

		for (int i = dstMip; i <= lastMipLevel; i++) //i is index of destination texture
		{
			if (i == dstMip) //instead of previous texture there should be an HDR buffer
			{
				currentTexture = &tex;
			}
			else
			{
				currentTexture = &textures[i - 1];
			}
			const vec2 texelSize = { 
				1.f / Math::max(currentTexture->getSize().x, 1u), 
				1.f / Math::max(currentTexture->getSize().y, 1u) };
			downscale->setUniform(downscale->getLocation("tex"), *currentTexture, 0);
			downscale->setUniform(downscale->getLocation("texelSize"), texelSize);
			fb.setTexture(0, textures[i]);
			fb.use();
			VertexBuffer::getDefFramebuf().draw();
		}
	}
	void BloomEffect::useUpscale(int mipStart, int mipEnd, BlendMode mode)
	{
		Pipeline::setBlendMode(mode);

		const Texture2D* currentTexture = nullptr;
		upscale->use();
		for (int i = (int)mipStart; i >= mipEnd; i--)
		{
			currentTexture = &textures[i + 1];
			const vec2 filterRadius = vec2{ 1.f / currentTexture->getSize().x, 1.f / currentTexture->getSize().y } *radius;
			upscale->setUniform(upscale->getLocation("tex"), *currentTexture, 0);
			upscale->setUniform(upscale->getLocation("radius"), filterRadius);
			fb.setTexture(0, textures[i]);
			fb.use();
			VertexBuffer::getDefFramebuf().draw();
		}
	}
}
