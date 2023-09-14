#pragma once

#include "Age/types.hpp"
#include "Age/Math/vec2.hpp"
#include "Age/Light/Light.hpp"

namespace a_game_engine
{
	class Texture2D;
	class CubeMap;
	struct mat3;
	struct mat4;
	struct ColorMaterial;
	struct TextureMaterial;
	class Camera3D;
	class Node3D;

	class ShaderProgram
	{
		uint _id = 0;
		bool _isValid = false;

		void destroy();

		int location(const char* name) const;
	public:
		static const uint MAX_LIGHT_SOURCES = 32;

		ShaderProgram() = default;
		ShaderProgram(uint vert, uint frag);
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) noexcept;
		ShaderProgram& operator=(ShaderProgram&&) noexcept;
		~ShaderProgram();

		void create(uint vert, uint frag);
		inline uint getId() const { return _id; }
		bool isValid() const;

		void setUniform(const char* name, int value) const;
		void setUniform(const char* name, float value) const;
		void setUniform(const char* name, vec2 value) const;
		void setUniform(const char* name, vec3 value) const;
		void setUniform(const char* name, const mat3& value) const;
		void setUniform(const char* name, const mat4& value) const;
		void setUniform(const char* name, const Texture2D& value, uint number) const;
		void setUniform(const char* name, const CubeMap& value, uint number) const;
		uint setUniform(const char* name, const TextureMaterial& mat, uint firstSampler) const;
		void setUniform(const char* name, const DirLight& light) const;
		void setUniform(const char* name, const PointLight& light) const;
		void setUniform(const char* name, const SpotLight& light) const;
		void setUniform(const char* name, const ColorMaterial& mat) const;
		void setLights(const Node3D& scene) const;
		void setCamera(const Camera3D& camera) const;

		void use() const;
		static void use(uint id);
	};
}