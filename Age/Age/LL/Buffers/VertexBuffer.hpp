#pragma once

#include "Age/types.hpp"
#include <vector>

namespace a_game_engine
{
	class VertexBuffer
	{
		uint _id, _indexBuffer = 0;
		std::vector<uint> _buffers;
	public:
		int vertCount = 0;
		int instanceCount = 1;

		enum LoadMode : int { Static, Stream, Dynamic };

		struct Attributes 
		{
			uint attribute = 0, units = 0, stride = 0, offset = 0;

			Attributes() = default;
			Attributes(uint attribute, uint units, uint stride, uint offset);

			static std::vector<Attributes> getAttributes(const std::vector<uint>& buffer);
		};

		enum DrawMode : int { Points, Lines, LineLoop, LineStrip, Triangles, TriangleStrip, TriangleFan };

		VertexBuffer();
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;
		~VertexBuffer();

		inline uint getId() const { return _id; }

		void addFloatBuffer(const float* data, size_t size, 
			Attributes attributes, LoadMode loadMode = Static);
		void addFloatBuffer(const float* data, size_t size, 
			const std::vector<Attributes>& attributes, LoadMode loadMode = Static);
		void addFloatBuffer(const float* data, size_t size, uint units, LoadMode loadMode = Static);
		void addFloatBuffer(const float* data, size_t size,
			const std::vector<uint>& units, LoadMode loadMode = Static);
		void setIndexBuffer(const std::vector<uint>& buffer, LoadMode loadMode = Static);

		void addInstanceFloatBuffer(const float* data, size_t size,
			const std::vector<Attributes>& attributes, LoadMode loadMode = Static);
		void addInstanceFloatBuffer(const float* data, size_t size,
			const std::vector<uint>& units, LoadMode loadMode = Static);

		void bind() const;

		void draw(DrawMode drawMode, int begin, int count) const;
		void draw(DrawMode drawMode = Triangles) const;

		inline const std::vector<uint> getVboIds() const { return _buffers; }

		static const VertexBuffer& getDefFramebuf();
		static const VertexBuffer& getQuadFramebuffer();
	};
}
