#include "pch.hpp"
#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const std::uint32_t* indices, const std::uint32_t count)
	:
	m_count(count) // needed for glDrawElements
{
	glAssert(glGenBuffers(1, &m_id));
	this->Bind();
	glAssert(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(decltype(indices[0])), indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	glAssert(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::Bind() const noexcept
{
	glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void IndexBuffer::Unbind() const noexcept
{
	glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}