#pragma once

#include <string>
#include "Age/types.hpp"
#include <map>
#include <memory>
#include <vector>

namespace a_game_engine
{
	class ShaderProgram;

	class Shader
	{
	public:
		struct ForwardSettings
		{
			uint dirLights = 0, pointLights = 0, spotLights = 0;

			bool operator<(const ForwardSettings& other) const;
			bool operator==(const ForwardSettings& other) const;
		};
		struct DeferredSettings
		{
			std::vector<uint> bindings;
			std::string paintingFunc;

			bool operator<(const DeferredSettings& other) const;
			bool operator==(const DeferredSettings& other) const;
		};
		struct DetailedSettings
		{
			std::string defines, commonLib, fragmentLib, vertexLib, forcePaintingOver, mainFunc;
			std::vector<uint> bindings;
		};
		struct Settings
		{
			enum Type
			{
				Undefined, Forward, Deferred
			} type = Undefined;
			union AnySettings{
				ForwardSettings forward;
				DeferredSettings deferred;

				inline AnySettings() {}
				inline ~AnySettings() {}
			} settings;

			Settings();
			Settings(const ForwardSettings& s);
			Settings(const DeferredSettings& s);

			bool operator==(const Settings& other) const;
			Settings& operator=(const Settings& s);
		};
	private:
		std::string _source;

		using ShaderPtr = const ShaderProgram*;
		mutable Settings _lastSettings;
		mutable ShaderPtr _lastShader = nullptr;

		mutable std::map<DeferredSettings, std::unique_ptr<ShaderProgram>> _deferred;
		mutable std::map<ForwardSettings, std::unique_ptr<ShaderProgram>> _forward;

		static std::string generateBinding(uint vecComponents, uint number);
		static std::string toVecType(uint vecComponents);
	public:
		Shader(const std::string& source);

		const ShaderProgram& getForward(const ForwardSettings& s) const;
		const ShaderProgram& getDeferred(const DeferredSettings& s) const;
		const ShaderProgram& getProgram(const Settings& s) const;
		static std::unique_ptr<ShaderProgram> createProgram(const DetailedSettings& s, const std::string& source);
	};
}
