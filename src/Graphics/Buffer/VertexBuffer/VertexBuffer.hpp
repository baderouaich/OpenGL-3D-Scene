#pragma once
#include <Utility/OpenGLUtils.hpp>
#include <Graphics/Buffer/BufferLayout/BufferLayout.hpp>
#include <glad/glad.h>
#include <memory>
#include <Graphics/Buffer/Vertex/Vertex.hpp>
#include <span>

/*
VBO p29 s5.2
We manage GPU memory via so called vertex buffer objects (VBO) that can store a large number
of vertices in the GPU?s memory. The advantage of using those buffer objects is that we can send
large batches of data all at once to the graphics card, and keep it there if there?s enough memory left,
without having to send data one vertex at a time. Sending data to the graphics card from the CPU is
relatively slow, so wherever we can we try to send as much data as possible at once. Once the data is
in the graphics card?s memory the vertex shader has almost instant access to the vertices making it
extremely fast

also p34 s5

VertexBuffer is an array of vertices [(x,y,z),(x,y,z)...]
used to pack vertices for some object like a bunny 3D ? a 2D triangle.. or a part of an object
*/
class VertexBuffer
{
public:
    /*
    VBO p29 s5.2
    The fourth parameter specifies how we want the graphics card to manage the given data. This
    can take 3 forms:
    ? GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    ? GL_STATIC_DRAW: the data is set only once and used many times.
    ? GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    */
    enum class DrawType : GLenum
    {
        Static = GL_STATIC_DRAW,
        Dynamic = GL_DYNAMIC_DRAW,
        Stream = GL_STREAM_DRAW,
    };

public:
    explicit VertexBuffer(DrawType draw_type);
    ~VertexBuffer();

public:
    //VertexBuffer& Bind() const noexcept;
    //VertexBuffer& Unbind() const noexcept;
    void Bind() const noexcept;
    void Unbind() const noexcept;

public:
    //VertexBuffer& SetBufferData(const float* vertices, const GLsizeiptr size);
    //VertexBuffer& SetLayout(const BufferLayout& layout) noexcept { m_layout = layout; }
    void SetBufferData(const float* vertices, const GLsizeiptr size);
    void SetBufferData(const std::span<Vertex>& vertices);
    void SetBufferData(const Vertex* vertices, const GLsizeiptr count);
    void SetLayout(const BufferLayout& layout) noexcept { m_layout = layout; }
    const BufferLayout& GetLayout() const noexcept { return m_layout; }

private:
#if 0
    bool IsBound()
	{
		//https://stackoverflow.com/questions/27459859/how-to-check-which-frame-buffer-object-is-currently-bound-in-opengl
		GLint r{ 0 };
		glAssert(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &r));
		return r == m_id;
	}
#endif

private:
    GLuint m_id{};
    DrawType m_draw_type{};
    BufferLayout m_layout{};
};

/*

Create a class representation of a
 */