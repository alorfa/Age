#pragma once

#include "ShaderSettings.hpp"
#include <map>
#include <memory>

namespace a_game_engine
{
	class ShaderProgram;

	class Shader
	{
		std::string _source;
		const ShaderSettings::Include* _include;

		using ShaderPtr = const ShaderProgram*;
		mutable ShaderSettings::Common _lastSettings;
		mutable ShaderPtr _lastShader = nullptr;

		mutable std::map<ShaderSettings::Forward, std::unique_ptr<ShaderProgram>> _forward;
		mutable std::map<ShaderSettings::Deferred, std::unique_ptr<ShaderProgram>> _deferred;

		static std::string generateBinding(uint vecComponents, uint number);
		static std::string toVecType(uint vecComponents);
	public:
		Shader(const std::string& source, const ShaderSettings::Include& include);
		Shader(const std::string& source);
		struct GlslCode { std::string vert, frag; };

		const ShaderProgram& getForward(const ShaderSettings::Forward& s) const;
		const ShaderProgram& getDeferred(const ShaderSettings::Deferred& s) const;
		const ShaderProgram& getProgram(const ShaderSettings::Common& s) const;
		static std::unique_ptr<ShaderProgram> createProgram(const ShaderSettings::Detailed& s, const std::string& source);
		static GlslCode translateToGlsl(const ShaderSettings::Detailed& s, const std::string& source);
	};
}
