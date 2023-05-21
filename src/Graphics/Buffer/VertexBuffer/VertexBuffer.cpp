#include "pch.hpp"
#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(DrawType draw_type)
        :
        m_draw_type(draw_type)
{
  glAssert(glGenBuffers(1, &m_id));
}

VertexBuffer::~VertexBuffer()
{
  glAssert(glDeleteBuffers(1, &m_id));
}

void VertexBuffer::Bind() const noexcept
{
  glAssert(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VertexBuffer::Unbind() const noexcept
{
  glAssert(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::SetBufferData(const float* vertices, const GLsizeiptr size)
{
  this->Bind();
  // p29 s5.2 /We call to the glBufferData function that copies the previously defined vertex data into the buffers memory GPU
  glAssert(glBufferData(GL_ARRAY_BUFFER, size, vertices, static_cast<GLenum>(m_draw_type)));
}

void VertexBuffer::SetBufferData(const std::span<Vertex> &vertices) {
  this->Bind();
  glAssert(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), static_cast<GLenum>(m_draw_type)));
}

void VertexBuffer::SetBufferData(const Vertex *vertices, const GLsizeiptr count) {
  this->Bind();
  glAssert(glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), vertices, static_cast<GLenum>(m_draw_type)));
}
