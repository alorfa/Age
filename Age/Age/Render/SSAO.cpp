#include "SSAO.hpp"
#include <random>
#include "Age/Math/Math.hpp"
#include "Age/egd.hpp"
#include <format>

namespace a_game_engine
{
    std::vector<vec3> SSAO::kernel;
    Texture2D SSAO::noise;    

    SSAO::SSAO()
    {
        ssaoPass = &egd.shaders.loadPostproc(egd.res / "shader/gbuffer/ssao.pasl");
        //ssaoBlur = &egd.shaders.loadPostproc(egd.res / "shader/ssaoBlur.pasl");

        const auto& kernel = getKernel();
        for (int i = 0; i < KERNEL_SIZE; i++)
            ssaoPass->setUniform(ssaoPass->getLocation(std::format("kernel[{}]", i).c_str()), kernel[i]);

        fb.setTexture(0, ssaoBuffer);
    }
    void SSAO::create(uvec2 size)
    {
        ImageInfo img;
        img.format = TextureFormat::R_F32;
        img.size = size;
        ssaoBuffer.create(Texture::Settings{ img, TextureFormat::AutoQuality, {}, 1 });
    }
    void SSAO::use(int posMapSlot, int normalMapSlot, int noiseSlot, const mat4& projMatrix)
    {
        ssaoPass->use();
        ssaoPass->setUniform(ssaoPass->getLocation("pos_map"), posMapSlot);
        ssaoPass->setUniform(ssaoPass->getLocation("normal_map"), normalMapSlot);
        ssaoPass->setUniform(ssaoPass->getLocation("noise_map"), noiseSlot);
        ssaoPass->setUniform(ssaoPass->getLocation("proj"), projMatrix);

        fb.use();
        VertexBuffer::getDefFramebuf().draw();
    }

    const std::vector<vec3>& SSAO::getKernel()
    {
        if (kernel.size() == 0)
        {
            kernel.resize(KERNEL_SIZE);
            std::uniform_real_distribution<float> randomFloats{ 0.f, 1.f };
            std::default_random_engine generator;
            for (int i = 0; i < KERNEL_SIZE; i++)
            {
                vec3 sample(
                    randomFloats(generator) * 2.f - 1.f,
                    randomFloats(generator) * 2.f - 1.f,
                    randomFloats(generator)
                );
                sample.normalize();
                sample *= randomFloats(generator);
                float scale = (float)i / (float)KERNEL_SIZE;
                scale = Math::lerp(0.1f, 1.f, scale * scale);
                kernel[i] = sample * scale;
            }
        }
        return kernel;
    }
    const Texture2D& SSAO::getNoise()
    {
        if (not noise.isValid())
        {
            std::uniform_real_distribution<float> randomFloats{ 0.f, 1.f };
            std::default_random_engine generator;

            std::vector<vec2> data;
            data.resize(TEXTURE_SIZE * TEXTURE_SIZE);
            for (int i = 0; i < TEXTURE_SIZE * TEXTURE_SIZE; i++)
            {
                data[i] = vec2{
                    randomFloats(generator) * 2.f - 1.f,
                    randomFloats(generator) * 2.f - 1.f
                };
            }
            ImageInfo img;
            img.data = (ubyte*)data.data();
            img.format = TextureFormat::RG_F32;
            img.size = { TEXTURE_SIZE, TEXTURE_SIZE };
            Sampler2DInfo sampler = { TextureFiltering::Near, TextureWrap::Repeat };
            noise.create(Texture::Settings{ img, TextureFormat::AutoQuality, sampler, 1 });
        }
        return noise;
    }
}
