#include "pch.hpp"
#include "VertexArray.hpp"

VertexArray::VertexArray()
{
	//glCreateVertexArrays(1, &m_id); GL >= 4.5
	glAssert(glGenVertexArrays(1, &m_id));
}

VertexArray::~VertexArray()
{
	glAssert(glDeleteVertexArrays(1, &m_id));
}

void VertexArray::Bind() const noexcept
{
	glAssert(glBindVertexArray(m_id));
}

void VertexArray::Unbind() const noexcept
{
	glAssert(glBindVertexArray(0));
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer)
{
	this->Bind(); // Bind Current VAO 
	vertex_buffer->Bind(); // Bind VBO

	assert(not vertex_buffer->GetLayout().GetElements().empty() && "Vertex Buffer has no layout");

	std::uint32_t index = 0;
	auto& layout = vertex_buffer->GetLayout();
	for (const auto& element : layout)
	{
		glAssert(glEnableVertexAttribArray(index));

		glAssert(glVertexAttribPointer(
			index,
			element.Count(),
			Shader::ShaderDataTypeToOpenGLBaseType(element.type),
			element.normalized ? GL_TRUE : GL_FALSE,
			layout.GetStride(),
			(const void*)element.offset)
		);

		index++;
	}


	m_vertex_buffers.emplace_back(vertex_buffer);

}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) noexcept
{
	this->Bind();
	index_buffer->Bind();

	m_index_buffer = index_buffer;
}
