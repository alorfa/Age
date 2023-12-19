#pragma once

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
			std::string vertLib, vertInc, vertMain, fragLib, fragInc, fragMain;
		};
		struct Forward
		{
			uint dirLights = 0, pointLights = 0, spotLights = 0;

			bool operator<(const Forward& other) const;
			bool operator==(const Forward& other) const;
		};
		struct DeferredImpl
		{
			std::vector<uint> bindings;
			std::string paintingFunc;
		};
		struct Deferred
		{
			int implIndex = -1;

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
			int definesIndex = -1;

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
			const std::string* paintingFunc = nullptr;
			std::vector<uint> bindings;

			Detailed(const Include& include);
			Detailed(const Include& include, const Forward& f);
			Detailed(const Include& include, const Deferred& d);
			Detailed(const Include& include, const Common& c);
			void create(const Forward& f);
			void create(const Deferred& f);
			void create(const Common& f);
		};

		static Include include, rawInclude;
		static std::vector<DeferredImpl> deferredImpls;
		static std::vector<std::string> additionalDefines;
		static std::string postprocVsh;
	};
}
