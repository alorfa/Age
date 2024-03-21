#pragma once

#include "Age/types.hpp"
#include "Age/Math/vec2.hpp"
#include "Age/Light/Light.hpp"
#include "ShaderModule.hpp"

namespace a_game_engine
{
	class Texture2D;
	class CubeMap;
	class Camera;
	class Node;
	class ShaderProperty;
	class MaterialProperty;

	struct mat3;
	struct mat4;
	struct TransformProps;

	class ShaderProgram
	{
		uint _id = 0;
		bool _isValid = false;

		void destroy();

	public:
		int getLocation(const char* name) const;

		ShaderProgram() = default;
		ShaderProgram(const ShaderModule& vert, const ShaderModule& frag);
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) noexcept;
		ShaderProgram& operator=(ShaderProgram&&) noexcept;
		~ShaderProgram();

		void create(const ShaderModule& vert, const ShaderModule& frag);
		inline uint getId() const { return _id; }
		bool isValid() const;

		void setUniform(int location, int value) const;
		void setUniform(int location, float value) const;
		void setUniform(int location, vec2 value) const;
		void setUniform(int location, vec3 value) const;
		void setUniform(int location, const mat3& value) const;
		void setUniform(int location, const mat4& value) const;
		void setUniform(int location, const Texture2D& value, uint number) const;
		void setUniform(int location, const CubeMap& value, uint number) const;
		void setUniform(int location, const ShaderProperty& value) const;
		void setUniform(const MaterialProperty& value) const;
		void setCamera(const Camera& camera) const;
		void setTransformProps(const TransformProps& props) const;

		void use() const;
	};
}