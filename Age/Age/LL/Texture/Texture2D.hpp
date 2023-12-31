#pragma once

#include "Age/types.hpp"
#include "Age/Math/vec2.hpp"
#include "Age/Math/vec3.hpp"
#include "TexEnums.hpp"

namespace a_game_engine
{
	class Texture2D
	{
		uint _id = 0;
		uvec2 _size;
	public:
		struct Settings
		{
			ImageInfo img;
			TextureFormat internal;
			bool mipmaps = true;

			Settings(const ImageInfo& img, TextureFormat internal, bool mipmaps = true);
			Settings(const ImageInfo& img, bool mipmaps = true);
		};

		Texture2D() = default;
		~Texture2D();
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D(Texture2D&&) noexcept;
		Texture2D& operator=(Texture2D&&) noexcept;
		Texture2D(const Settings& s);
		void create(const Settings& s);
		void generateMipmaps();
		void destroy();
		vec3 getMidColor() const;

		void setWrap(TextureWrap x, TextureWrap y);
		void setWrap(TextureWrap wrap);
		void setFiltering(TextureFiltering min, TextureFiltering mag);
		void setFiltering(TextureFiltering filter);

		inline uint getId() const { return _id; }
		inline const uvec2& getSize() const { return _size; }
		void activate(int number = 0) const;
	};

	using Texture = Texture2D;
}