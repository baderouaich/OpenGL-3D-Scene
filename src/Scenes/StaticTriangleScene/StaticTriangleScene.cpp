#include "pch.hpp"
#include "StaticTriangleScene.hpp"
#include <Application/Application.hpp>
#include <memory>
#include <Graphics/Shader/Shader.hpp>
#include <Graphics/Buffer/VertexArray/VertexArray.hpp>
#include <Graphics/Buffer/VertexBuffer/VertexBuffer.hpp>
#include <glad/glad.h>

/* OpenGL Triangle Drawing Steps:
* # Initialization:
* 1- Create VertexArray object glGenVertexArrays(1, &m_id);
* 2- Create VertexBuffer objects glGenBuffers(1, &m_id);
* 3- Bind and Set vertices buffer to VertexBuffer objects  glBindBuffer(GL_ARRAY_BUFFER, m_id) & glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW|GL_DYNAMIC_DRAW))
* 4- Create Set BufferLayout to VertexBuffer, to tell OpenGL what are my vertices contain of? positions and colors? or positions only...and their types
* 5- Add Created VertexBuffer objects to VertexArray m_vertex_array->AddVertexBuffer(vertex_buffer);
* 6- Create & Bind IndexBuffer object, to tell OpenGL how to draw vertices in order from 0 to N vertices  glGenBuffers(1, &m_id); && glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id) && glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(decltype(indices[0])), indices, GL_STATIC_DRAW);
* 7- Set IndexBuffer object to VertexArray (bind vao, bind ibo)
* 8- Create Shader program of vertex and fragment, with layout(location = 0) in vec3 a_Position; in vertex according to your vertices layout, and uniform vec4 u_Color; if u want to upload them from cpp
* # Draw
* 1- Clear buffers glClearColor(0.2f, 0.3f, 0.3f, 1.0f) && glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)
* 2- Bind & set uniform u_Color to m_triangle_color which modifies by ui settings m_shader->Bind(); && m_shader->SetVec4f("u_Color", m_triangle_color);
* 3- Bind vertex array to be drawn m_vertex_array->Bind();
* 4- Draw triangle glDrawElements(GL_TRIANGLES, m_vertex_array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
* 5- Unbind Shader & VAO
*/



// -1.0f -> 1.0f coords
static const GLfloat vertices[]  = {
        -0.5f, -0.5f * sqrtf(3.0f) / 3.0f, 0.0f,
        0.5f, -0.5f * sqrtf(3.0f) / 3.0f, 0.0f,
        0.0f, 0.5f * sqrtf(3.0f) * 2.0f / 2.0f, 0.0f,
};
static const std::vector<GLuint> indices ={
        0, 1, 2
};
static std::shared_ptr<Shader> shader;
static std::shared_ptr<VertexBuffer> VBO;
static std::shared_ptr<IndexBuffer> IBO;
static std::shared_ptr<VertexArray> VAO;

static glm::vec4 mColor(1.0, 1.0, 1.0, 1.0);


void StaticTriangleScene::OnCreate()
{
	TRACE_FUNCTION();

  shader.reset(new Shader({
      {Shader::ShaderType::Vertex, Shader::ShaderSourceType::File, "res/shaders/triangle/triangle.vert"},
      {Shader::ShaderType::Fragment, Shader::ShaderSourceType::File,"res/shaders/triangle/triangle.frag"},
  }));

  VAO.reset(new VertexArray());
  VAO->Bind();

  VBO.reset(new VertexBuffer(VertexBuffer::DrawType::Static));
  VBO->Bind();

  IBO.reset(new IndexBuffer(indices.data(), indices.size()));

  VBO->SetBufferData(vertices, sizeof(vertices));
  VBO->SetLayout(BufferLayout({ /* layout(location = 0) in vec3 a_Position; */
    BufferElement(Shader::ShaderDataType::Vec3f, "a_Position"), // layout (location = 0) in vec3 mPos; // the position variable has attribute position 0
  }));


  VAO->SetIndexBuffer(IBO);
  VAO->AddVertexBuffer(VBO);
}

void StaticTriangleScene::OnEvent([[maybe_unused]] Event& event)
{
  EventDispatcher dispatcher(event);
  // Handle Keyboard ESCAPE Press
  dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) -> bool
             {
                 if (e.GetKeyCode() == KeyCode::Escape)
                 {
                   Scene::Exit();
                   return true;
                 }
                 return false;
             });
}

void StaticTriangleScene::OnUpdate([[maybe_unused]] float dt)
{

}

void StaticTriangleScene::OnDraw()
{
  // Clear buffers
  glAssert(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
  glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

  // Bind & set uniform u_Color to m_triangle_color which modifies by ui settings
  shader->Bind();
  shader->SetVec4f("mColor", mColor); // uniform vec4 mColor;

  // Bind vertex array to be drawn
  VAO->Bind();

  // Draw triangle
  glAssert(glDrawElements(GL_TRIANGLES, VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));

  // Unbind stuff
  shader->Unbind();
  VAO->Unbind();
}

void StaticTriangleScene::OnImGuiDraw()
{
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Triangle Color [uniform vec4 mColor;]", glm::value_ptr(mColor));
  ImGui::End();
}

void StaticTriangleScene::OnDestroy()
{
	TRACE_FUNCTION();

  shader->Unbind();
  VBO->Unbind();
  IBO->Unbind();
  VAO->Unbind();
}
