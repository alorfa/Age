#include "Age/types.hpp"
#include <vector>
#include <string>
#include <variant>

namespace a_game_engine
{
	class Shader;

	class ShaderSettings
	{
		friend class Shader;
	public:
		struct Include
		{
			std::string common, vertInc, fragInc, vertMain, fragMain;
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
			int paintingFuncIndex = -1;

			bool operator<(const Deferred& other) const;
			bool operator==(const Deferred& other) const;
		};
		struct Common
		{
			enum class Type
			{
				Undefined, Forward, Deferred
			} type = Type::Undefined;
			std::variant<Forward, Deferred> settings;

			Common();
			Common(const Forward& s);
			Common(const Deferred& s);

			bool operator==(const Common& other) const;
			Common& operator=(const Common& s);
		};
		struct Detailed
		{
			const Include* include;
			std::string defines;
			int paintingFuncIndex = -1;
			std::vector<uint> bindings;

			Detailed(const Include& include);
			Detailed(const Include& include, const Forward& f);
			Detailed(const Include& include, const Deferred& d);
			Detailed(const Include& include, const Common& c);
			void create(const Forward& f);
			void create(const Deferred& f);
			void create(const Common& f);
		};

		static Include include;
		static std::vector<std::string> paintingFunctions;
	private:
	};
}
