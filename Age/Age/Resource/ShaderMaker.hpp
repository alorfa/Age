#pragma once

#include <string>
#include <Age/types.hpp>
#include <vector>

namespace a_game_engine
{
	class VertexShaderMaker
	{
	public:
		struct Params3D
		{
			bool hasNormals = true;
			uint uvComponents = 2;
			bool passPos = true;
			bool safeScaleNormals = false;
			std::string customBody;
		};
		struct Exception {};

		static std::string compsToFloatType(uint comps);
		static std::string createHeader(const Params3D& params);
		static std::string createBody(const Params3D& params);
		static std::string create(const Params3D& params);
	};

	class FragmentShaderMaker
	{
	public:
		struct Params3D
		{

		};
	};
}