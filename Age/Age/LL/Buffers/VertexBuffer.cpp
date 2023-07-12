#include "VertexBuffer.hpp"
#include "Age/LL/opengl.h"
#include <memory>

namespace a_game_engine
{
	namespace
	{
		int toOglType(VertexBuffer::LoadMode m)
		{
			switch (m)
			{
			case VertexBuffer::Static:
			case GL_STATIC_DRAW:
				return GL_STATIC_DRAW;
			case VertexBuffer::Dynamic:
			case GL_DYNAMIC_DRAW:
				return GL_DYNAMIC_DRAW;
			case VertexBuffer::Stream:
			case GL_STREAM_DRAW:
				return GL_STREAM_DRAW;
			}
			return GL_STATIC_DRAW;
		}
		int toOglType(VertexBuffer::DrawMode m)
		{
			switch (m)
			{
			case VertexBuffer::Points:
				return GL_POINTS;
			case VertexBuffer::Lines:
				return GL_LINES;
			case VertexBuffer::LineLoop:
				return GL_LINE_LOOP;
			case VertexBuffer::LineStrip:
				return GL_LINE_STRIP;
			case VertexBuffer::Triangles:
				return GL_TRIANGLES;
			case VertexBuffer::TriangleStrip:
				return GL_TRIANGLE_STRIP;
			case VertexBuffer::TriangleFan:
				return GL_TRIANGLE_FAN;
			}
			return GL_TRIANGLES;
		}
	}

	VertexBuffer::Attributes::Attributes(uint attribute, uint units, uint stride, uint offset)
	{
		this->units = units;
		this->stride = stride;
		this->offset = offset;
		this->attribute = attribute;
	}

	VertexBuffer::VertexBuffer()
	{
		glGenVertexArrays(1, &_id);
	}
	VertexBuffer::~VertexBuffer()
	{
		if (_buffers.size())
			glDeleteBuffers((int)_buffers.size(), _buffers.data());
		if (_indexBuffer)
			glDeleteBuffers(1, &_indexBuffer);
		if (_id)
			glDeleteVertexArrays(1, &_id);
	}
	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
		: _id(other._id), _indexBuffer(other._indexBuffer), 
		_buffers(std::move(other._buffers)), vertCount(other.vertCount)
	{
		other._id = 0;
		other._indexBuffer = 0;
		other.vertCount = 0;
	}
	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
	{
		std::swap(_id, other._id);
		std::swap(_indexBuffer, other._indexBuffer);
		std::swap(_buffers, other._buffers);
		std::swap(vertCount, other.vertCount);

		return *this;
	}
	void VertexBuffer::bind() const
	{
		glBindVertexArray(_id);
	}
	void VertexBuffer::addFloatBuffer(const float* data, size_t size, Attributes attributes, LoadMode loadMode)
	{
		addFloatBuffer(data, size, std::vector<Attributes>{ attributes }, loadMode);
	}
	void VertexBuffer::addFloatBuffer(const float* data, size_t size, 
		const std::vector<Attributes>& attributes, LoadMode loadMode)
	{
		bind();
		uint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, toOglType(loadMode));

		for (int i = 0; i < attributes.size(); i++)
		{
			auto& attr = attributes[i];
			glVertexAttribPointer(attr.attribute, (int)attr.units, GL_FLOAT, GL_FALSE,
				attr.stride * sizeof(float), (void*)(attr.offset * sizeof(float)));
			glEnableVertexAttribArray(attr.attribute);
		}
		_buffers.push_back(vbo);
	}
	void VertexBuffer::addFloatBuffer(const float* data, size_t size, uint units, LoadMode loadMode)
	{
		addFloatBuffer(data, size, Attributes{ 0, units, units, 0 }, loadMode);
	}
	void VertexBuffer::addFloatBuffer(const float* data, size_t size, 
		const std::vector<uint>& units, LoadMode loadMode)
	{
		std::vector<Attributes> atr{units.size()};
		int stride = 0;
		for (int i = 0; i < units.size(); i++)
			stride += units[i];

		for (int i = 0; i < units.size(); i++)
		{
			atr[i].attribute = i;
			atr[i].units = units[i];
			atr[i].stride = stride;
			if (i > 0)
				atr[i].offset = atr[i - 1].units + atr[i - 1].offset;
		}
		addFloatBuffer(data, size, atr, loadMode);
	}

	void VertexBuffer::setIndexBuffer(const std::vector<uint>& buffer, LoadMode loadMode)
	{
		bind();
		uint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer.data(), toOglType(loadMode));

		if (_indexBuffer)
			glDeleteBuffers(1, &_indexBuffer);
		_indexBuffer = ebo;
		vertCount = (int)buffer.size();
	}
	void VertexBuffer::draw(DrawMode mode, int begin, int vertCount) const
	{
		bind();
		if (_indexBuffer)
			glDrawElements(toOglType(mode), vertCount, GL_UNSIGNED_INT, (void*)((size_t)begin));
		else
			glDrawArrays(toOglType(mode), begin, vertCount);
	}
	void VertexBuffer::draw(DrawMode mode) const
	{
		draw(mode, 0, vertCount);
	}
	const VertexBuffer& VertexBuffer::getDefFramebuf()
	{
		static std::unique_ptr<VertexBuffer> result = nullptr;
		if (result == nullptr)
		{
			std::vector<float> vertices = {
				-1.f, -1.f,		0.f, 0.f,
				1.f, -1.f,		1.f, 0.f,
				-1.f, 1.f,		0.f, 1.f,
				-1.f, 1.f,		0.f, 1.f,
				1.f, -1.f,		1.f, 0.f,
				1.f, 1.f,		1.f, 1.f
			};
			result = std::make_unique<VertexBuffer>();
			result->addFloatBuffer(vertices.data(), vertices.size(), { 2, 2 });
			result->vertCount = 6;
		}
		return *result;
	}
}