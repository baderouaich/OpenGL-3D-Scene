#pragma once
#include <Utility/OpenGLUtils.hpp>
#include <Graphics/Buffer/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/Buffer/BufferLayout/BufferLayout.hpp>
#include <Graphics/Buffer/IndexBuffer/IndexBuffer.hpp>
#include <memory>
#include <vector>

// p32 s5.5.1 Vertex Array Object (Array of VertexBuffer)
class VertexArray
{
public:
	VertexArray();
	~VertexArray();

public:
	void Bind() const noexcept;
	void Unbind() const noexcept;

public:
	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer);

	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) noexcept;

	const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const noexcept { return m_index_buffer; }
	const std::vector<std::shared_ptr<VertexBuffer>>& GetVertices() const noexcept { return m_vertex_buffers; }

private:
	GLuint m_id{};
	std::vector<std::shared_ptr<VertexBuffer>> m_vertex_buffers;
	std::shared_ptr<IndexBuffer> m_index_buffer;
};

