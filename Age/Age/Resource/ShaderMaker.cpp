#include "ShaderMaker.hpp"
#include <format>

namespace a_game_engine
{
	std::string VertexShaderMaker::compsToFloatType(uint comps)
	{
		switch (comps)
		{
		case 1:
			return "float";
		case 2:
			return "vec2";
		case 3:
			return "vec3";
		case 4:
			return "vec4";
		}
		throw Exception();
	}
	std::string VertexShaderMaker::createHeader(const Params3D& params)
	{
		try
		{
			std::string shader = "#version 330 core\n\n"
			"layout (location = 0) in vec3 inPosition;\n";
			if (params.hasNormals)
				shader += "layout (location = 1) in vec3 inNormal;\n";
			if (params.uvComponents)
				shader += std::format("layout (location = 2) in {} inUv;\n",
					compsToFloatType(params.uvComponents));
			shader += '\n';

			if (params.passPos)
				shader += "out vec3 fragPos;\n";
			if (params.hasNormals)
				shader += "out vec3 fragNormal;\n";
			if (params.uvComponents)
				shader += std::format("out {} fragUv;\n", compsToFloatType(params.uvComponents));
			shader += '\n';

			if (params.safeScaleNormals)
				shader += "uniform mat4 inversedModel;\n";
			shader += "uniform mat4 model, view, projection;\n\n";

			return shader;
		}
		catch (...)
		{
			return {};
		}
	}

	std::string VertexShaderMaker::createBody(const Params3D& params)
	{
		if (params.customBody.size())
			return params.customBody;

		std::string shader;
		if (params.passPos)
			shader += "fragPos = (model * vec4(inPosition, 1.f)).xyz;\n";
		if (params.hasNormals)
		{
			if (params.safeScaleNormals)
				shader += "fragNormal = mat3(transpose(inversedModel)) * inNormal;\n";
			else
				shader += "fragNormal = mat3(model) * inNormal;\n";
		}
		if (params.uvComponents)
			shader += "fragUv = inUv;\n";
		shader += "gl_Position = projection * view * model * vec4(inPosition, 1.f);\n";

		return shader;
	}
	std::string VertexShaderMaker::create(const Params3D& params)
	{
		std::string header = createHeader(params);
		if (header.empty())
			return {};

		std::string body = createBody(params);
		if (body.empty())
			return {};

		header += "void main\n{\n";
		header += body;
		header += '}';
		return header;
	}
}