#pragma once

#include <vector>
#include "Age/LL/Buffers/FrameBuffer2D.hpp"
#include "Age/LL/Shader/ShaderProgram.hpp"

namespace a_game_engine
{
	class SSAO
	{
		static std::vector<vec3> kernel;
		static Texture2D noise;

		FrameBuffer fb;
		Texture ssaoBuffer;

		const ShaderProgram
			*ssaoPass = nullptr, 
			*ssaoBlur = nullptr;
	public:
		static constexpr int KERNEL_SIZE = 64;
		static constexpr int TEXTURE_SIZE = 4;

		static const std::vector<vec3>& getKernel();
		static const Texture2D& getNoise();

		SSAO();
		void create(uvec2 size);
		void use(int posMapSlot, int normalMapSlot, int noiseSlot, const mat4& projMatrix);
	};
}
