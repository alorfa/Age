#include "Age/types.hpp"
#include <vector>
#include <string>

namespace a_game_engine
{
	class Shader;

	class ShaderSettings
	{
		friend class Shader;
	public:
		struct Include
		{
			std::string common, vertex, fragment, main;
		};
		struct Forward
		{
			uint dirLights = 0, pointLights = 0, spotLights = 0;

			bool operator<(const Forward& other) const;
			bool operator==(const Forward& other) const;
		};
		struct Deferred
		{
			std::vector<uint> bindings;
			std::string paintingFunc;

			bool operator<(const Deferred& other) const;
			bool operator==(const Deferred& other) const;
		};
		struct Detailed
		{
			const Include* include;
			std::string defines, forcePaintingOver;
			std::vector<uint> bindings;

			Detailed(const Include& include);
		};
		struct Common
		{
			enum class Type
			{
				Undefined, Forward, Deferred
			} type = Type::Undefined;
			union AnySettings {
				Forward forward;
				Deferred deferred;

				inline AnySettings() {}
				inline ~AnySettings() {}
			} settings;

			Common();
			Common(const Forward& s);
			Common(const Deferred& s);

			bool operator==(const Common& other) const;
			Common& operator=(const Common& s);
		};

		static Include include;
	private:
	};
}
