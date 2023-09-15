#pragma once

#include <string>
#include "Age/types.hpp"
#include <map>
#include <memory>

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
		};
	private:
		std::string _source;

		mutable std::unique_ptr<ShaderProgram> _deferred = nullptr;
		mutable std::map<ForwardSettings, std::unique_ptr<ShaderProgram>> _forward;
	public:
		Shader(const std::string& source);

		const ShaderProgram& getForward(const ForwardSettings& s) const;
		const ShaderProgram& getDeferred() const;
	};
}
