#pragma once

#include "Age/types.hpp"
#include "Age/Math/vec2.hpp"

namespace a_game_engine
{
	class Image;

	enum class TextureDataType
	{
		Ubyte, Float, Uint_24_8
	};
	enum class TextureFormat
	{ 
		Undefined = 0, R = 1, RG, RGB, RGBA,
		Depth24_Stencil8
	};
	enum class TextureFiltering
	{ 
		Linear, Near, 
		LinearMipLinear, LinearMipNear, 
		NearMipLinear, NearMipNear
	};
	enum class TextureWrap { Repeat, MirroredRepeat, ClampToEdge };

	class Texture2D
	{
		uint _id = 0;
		uvec2 _size;

		static int toOglFormat(TextureFormat);
		static int toOglFilter(TextureFiltering);
		static int toOglWrap(TextureWrap);
		static int toOglType(TextureDataType);
	public:
		struct Settings
		{
			uvec2 size;
			const ubyte* data;
			TextureFormat internal, outer;
			TextureDataType type;
			bool mipmaps = true;

			Settings(const uvec2& size, const ubyte* data, TextureFormat internal, TextureFormat outer,
				TextureDataType type = TextureDataType::Ubyte, bool mipmaps = true);
			Settings(const uvec2& size, const ubyte* data, TextureFormat format,
				TextureDataType type = TextureDataType::Ubyte, bool mipmaps = true);
		};

		Texture2D() = default;
		~Texture2D();
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D(Texture2D&&) noexcept;
		Texture2D& operator=(Texture2D&&) noexcept;
		Texture2D(const Settings& s);
		void create(const Settings& s);

		void setWrap(TextureWrap x, TextureWrap y);
		void setWrap(TextureWrap wrap);
		void setFiltering(TextureFiltering min, TextureFiltering mag);
		void setFiltering(TextureFiltering filter);

		inline uint getId() const { return _id; }
		inline const uvec2& getSize() const { return _size; }
		void activate(int number = 0) const;
	};
}