//
// Created by bader on 5/21/23.
//
#pragma once

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Graphics/Buffer/Vertex/Vertex.hpp"
#include "Primitives/Primitives.hpp"
#include "Graphics/Buffer/VertexArray/VertexArray.hpp"
#include "Graphics/Texture/Texture.hpp"



class Mesh {
//public:
//    struct MeshData {
//        std::string vertex_shader_file;
//        std::string fragment_shader_file;
//        std::string texture_file;
//        Primitive primitive; // any primitive with vertices data: cube, pyramid, sphere...
//    };

private:
    std::shared_ptr<Texture> m_texture;
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_IBO;

    glm::vec3 m_position;
    glm::vec3 m_origin; // origin position
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    glm::mat4 m_model_matrix; // model matrix

public:
    Mesh(const std::shared_ptr<Primitive>& primitive,
         glm::vec3 position = glm::vec3(0.0f),
         glm::vec3 origin = glm::vec3(0.0f),
         glm::vec3 rotation = glm::vec3(0.0f),
         glm::vec3 scale = glm::vec3(1.0f))
            :  Mesh(primitive->GetVertices(), primitive->GetIndices(), position, origin, rotation, scale)
    {

    }

    // Used when we load a .obj model
    Mesh(const std::vector<Vertex>& vertices,
            const std::vector<GLuint>& indices,
            glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 origin = glm::vec3(0.0f),
            glm::vec3 rotation = glm::vec3(0.0f),
            glm::vec3 scale = glm::vec3(1.0f))
            :
            m_position(position),
            m_origin(origin),
            m_rotation(rotation),
            m_scale(scale) // SCALE MUST BE 1.0F TO HAVE NO EFFECT BY DEFAULT
    {
      m_VAO.reset(new VertexArray());
      m_VAO->Bind();

      m_VBO.reset(new VertexBuffer(VertexBuffer::DrawType::Static));
      m_VBO->Bind();

      m_IBO.reset(new IndexBuffer(indices.data(), indices.size()));

      m_VBO->SetBufferData(vertices.data(), vertices.size());
      m_VBO->SetLayout(BufferLayout({
                                            /* Default shader default.vert & frag
                                        layout (location = 0) in vec3 position;
                                        layout (location = 1) in vec3 color;
                                        layout (location = 2) in vec2 texture_coords;
                                        layout (location = 3) in vec3 normal;*/
                                            BufferElement(Shader::ShaderDataType::Vec3f, "position"),
                                            BufferElement(Shader::ShaderDataType::Vec3f, "color"),
                                            BufferElement(Shader::ShaderDataType::Vec2f, "texture_coords"),
                                            BufferElement(Shader::ShaderDataType::Vec3f, "normal"),
                                    }));

      m_VAO->SetIndexBuffer(m_IBO);
      m_VAO->AddVertexBuffer(m_VBO);
    }

private:
    void UpdateUniforms(const std::shared_ptr<Shader>& shader)
    {
      shader->SetMat4("model_matrix", m_model_matrix, false);
    }

    void UpdateModelMatrix()
    {
      m_model_matrix = glm::mat4(1.0f);
      m_model_matrix = glm::translate(m_model_matrix, m_origin); // move ur model matrix
      m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // rotate ur model matrix by axis
      m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate ur model matrix by axis
      m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate ur model matrix by axis
      m_model_matrix = glm::translate(m_model_matrix, m_position - m_origin); // move to offset
      m_model_matrix = glm::scale(m_model_matrix, m_scale); // scale your model matrix
    }

public:
    void Update(float dt) {
      //m_rotation.y += 0.1f * dt;
      UpdateModelMatrix();
    }

    void Draw(const std::shared_ptr<Shader>& shader)
    {
      UpdateUniforms(shader);
      //USE THE SHADER
      shader->Bind();

      //BIND VERTEX ARRAY OBJECT (THE BIG BOX)
      m_VAO->Bind();

      // WORKAROUND: Blender Exports by default Quads, Remember to import obj files with option 'triangulate faces'.
      // For me it took a lot of Time to realize that blender by default
      //	exports quads instead of triangles
      if(m_VAO->GetIndexBuffer()->GetCount() == 0) // if we're drawing vertices directly no index pointers, use glDrawArrays, otherwise use glDrawElements
        glAssert(glDrawArrays(GL_TRIANGLES, 0, m_VAO->GetVertices().size())); // <= This one draws all the vertices
      else
        glAssert(glDrawElements(GL_TRIANGLES, m_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr)); //0: starting from 0; <= this one draws only vertices pointed to in the index buffer

        //RESET/UNBIND EVERYTHING FOR NEW FRAME
        m_VAO->Unbind();
        shader->Unbind();

#if 0
        m_shader->Bind();
        m_shader->SetMat4("model", m_model_matrix, false);
        m_shader->SetMat4("projection", projection, false);
        m_shader->SetMat4("view", view, false);
        m_shader->SetTexture(m_texture->GetUniformName().c_str(), m_texture);
        m_VAO->Bind();

        // WORKAROUND: Blender Exports by default Quads, Remember to import obj files with option
        // 'triangulate faces'. For me, it took a lot of Time to realize that blender by default
        //	exports quads instead of triangles

        // Draw VAO
        if(m_VAO->GetIndexBuffer()->GetCount() == 0) // if we're drawing vertices directly no index pointers, use glDrawArrays, otherwise use glDrawElements
          glAssert(glDrawArrays(GL_TRIANGLES, 0, m_VAO->GetVertices().size())); // <= This one draws all the vertices
        else
          glAssert(glDrawElements(GL_TRIANGLES, m_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr)); //0: starting from 0; <= this one draws only vertices pointed to in the index buffer


        m_shader->Unbind();
        m_VAO->Unbind();
#endif
    }

public:
    //Modifiers
    void setPosition(const glm::vec3& position) { m_position = position; }
    void setOrigin(const glm::vec3& origin) { m_origin = origin; }
    void setRotation(const glm::vec3& rotation) { m_rotation = rotation; }
    void setScale(const glm::vec3& scale) { m_scale = scale; }

    glm::vec3& getRotation(){return m_rotation;}
    glm::vec3& getPosition(){return m_position;}


    void move(const glm::vec3& position) { m_position += position; }
    void rotate(const glm::vec3& rotation) { m_rotation += rotation; }
    void scale(const glm::vec3& scale) { m_scale += scale; }
};
#if 0
class Mesh
{
private:
    std::size_t m_nrOfVertices;
    std::size_t m_nrOfIndices;

    GLuint m_VAO; //VERTEX ARRAY OBJECT
    GLuint m_VBO; //VERTEX BUFFER OBJECT
    GLuint m_EBO; //IBO/EBO INDEX BUFFER OBJECT

    glm::vec3 m_position;
    glm::vec3 m_origin; // origin position
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    glm::mat4 m_model_matrix;
private:
    //Initialize Functions

    void InitVAO(Vertex* vertexArray,
                 const std::size_t& nrOfVertices,
                 GLuint* indexArray,
                 const std::size_t& nrOfIndices)
    {


      //VAO, VBO, EBO
      //GEN VAO AND BIND
      //~1: We put our box called VAO in front of us
      //VERTEX ARRAY OBJECT
      {
        glCreateVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO); // activate
      }
      //GEN VBO AND BIND AND SEND DATA
      //~2: We generate data and put it in the box
      //VERTEX BUFFER OBJECT
      {
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW); //send data to the GPU
      }
      //GEN IBO or EBO AND BIND AND SEND DATA
      //~3: Index buffer object to tell opengl which point in order to draw
      //INDEX BUFFER OBJECT
      if (m_nrOfIndices > 0)
      {
        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW); //send data to the GPU
      }


      //SET VERTEX ARRAY ATTRIB POINTERS AND ENABLE
      //~4: tell the GPU that vertices have position at first, colors then textcoords. gfx card has no idea about our data
      {

        /*
        glVertexAttribPointer
        (
        0: layout (location = 0) vertex_position (INPUT ASSEMBLY)
        3 floats, vertex_position has 3 floats x,y,z
        GL_FLASE, dont normalize,
        sizeof(Vertex): stride (how much we need to go in order to get to the next vertex position, its the size of the whole vertex,like going frol 1 to 2, takes 1 jump)
        (GLvoid*)offsetof(Vertex, position): size to get to next attribute (padding, alignment...) if used pack(0) size will be 0
        );
        */

        //# Position Attr #
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0); // 0: enable the attribute above which is the position

        //# Color Attr #
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1); // 1: enable the attribute above which is the color

        //# Texture Coord Attr #
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_coords));
        glEnableVertexAttribArray(2); // 2: enable the attribute above which is the texcoord

        //# Normal Coord Attr #
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(3); // 2: enable the attribute above which is the texcoord

      }

      //UNBIND VAO WHEN ~THE BOX IS PACKED
      glBindVertexArray(0);
    }

    //Init VAO with a primitive
    void InitVAO(const Primitive* primitive)
    {
      //VAO, VBO, EBO
      //GEN VAO AND BIND
      //~1: We put our box called VAO in front of us
      //VERTEX ARRAY OBJECT
      {
        glCreateVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO); // activate
      }
      //GEN VBO AND BIND AND SEND DATA
      //~2: We generate data and put it in the box
      //VERTEX BUFFER OBJECT
      {
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_nrOfVertices * sizeof(Vertex), primitive->GetVertices(), GL_STATIC_DRAW); //send data to the GPU
      }
      //GEN IBO or EBO AND BIND AND SEND DATA
      //~3: Index buffer object to tell opengl which point in order to draw
      //INDEX BUFFER OBJECT
      if(m_nrOfIndices > 0)
      {
        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nrOfIndices * sizeof(GLuint), primitive->GetIndices(), GL_STATIC_DRAW); //send data to the GPU
      }


      //SET VERTEX ARRAY ATTRIB POINTERS AND ENABLE
      //~4: tell the GPU that vertices have position at first, colors then textcoords. gfx card has no idea about our data
      {

        /*
        glVertexAttribPointer
        (
        0: layout (location = 0) vertex_position (INPUT ASSEMBLY)
        3 floats, vertex_position has 3 floats x,y,z
        GL_FLASE, dont normalize,
        sizeof(Vertex): stride (how much we need to go in order to get to the next vertex position, its the size of the whole vertex,like going frol 1 to 2, takes 1 jump)
        (GLvoid*)offsetof(Vertex, position): size to get to next attribute (padding, alignment...) if used pack(0) size will be 0
        );
        */

        //# Position Attr #
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0); // 0: enable the attribute above which is the position

        //# Color Attr #
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1); // 1: enable the attribute above which is the color

        //# Texture Coord Attr #
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_coords));
        glEnableVertexAttribArray(2); // 2: enable the attribute above which is the texcoord

        //# Normal Coord Attr #
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(3); // 2: enable the attribute above which is the texcoord

      }

      //UNBIND VAO WHEN ~THE BOX IS PACKED
      glBindVertexArray(0);
    }

private:
    void UpdateUniforms(Shader* shader)
    {
      shader->setMat4fv("ModelMatrix", m_model_matrix);
    }

    void UpdateModelMatrix()
    {
      m_model_matrix = glm::mat4(1.0f);
      m_model_matrix = glm::translate(m_model_matrix, m_origin); // move ur model matrix
      m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // rotate ur model matrix by axis
      m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate ur model matrix by axis
      m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate ur model matrix by axis
      m_model_matrix = glm::translate(m_model_matrix, m_position - m_origin); // move to offset
      m_model_matrix = glm::scale(m_model_matrix, m_scale); // scale your model matrix
    }

public:
    Mesh(
            Vertex* vertexArray,
            const std::size_t& nrOfVertices,
            GLuint* indexArray,
            const std::size_t& nrOfIndices,
            glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 origin = glm::vec3(0.0f),
            glm::vec3 rotation = glm::vec3(0.0f),
            glm::vec3 scale = glm::vec3(1.0f))
            :
            m_position(position),
            m_origin(origin),
            m_rotation(rotation),
            m_scale(scale),// SCALE MUST BE 1.0F TO HAVE NO EFFECT BY DEFAULT
            m_nrOfVertices(nrOfVertices),
            m_nrOfIndices(nrOfIndices)
    {
      InitVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
      UpdateModelMatrix();
    }

    Mesh(
            const Primitive* primitive,
            glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 origin = glm::vec3(0.0f),
            glm::vec3 rotation = glm::vec3(0.0f),
            glm::vec3 scale = glm::vec3(1.0f))
            :
            m_position(position),
            m_origin(origin),
            m_rotation(rotation),
            m_scale(scale),// SCALE MUST BE 1.0F TO HAVE NO EFFECT BY DEFAULT
            m_nrOfVertices(primitive->GetNrOfVertices()),
            m_nrOfIndices(primitive->GetNrOfIndices())

    {
      InitVAO(primitive);
      UpdateModelMatrix();
    }


    //Copy Constructor
    Mesh(const Mesh& mesh)
    {
      this->m_nrOfVertices = mesh.m_nrOfVertices;
      this->m_nrOfIndices = mesh.m_nrOfIndices;

      this->m_VAO = mesh.m_VAO; //VERTEX ARRAY OBJECT
      this->m_VBO = mesh.m_VBO; //VERTEX BUFFER OBJECT
      this->m_EBO = mesh.m_EBO; //IBO/EBO INDEX BUFFER OBJECT

      this->m_position = mesh.m_position;
      this->m_origin = mesh.m_origin;
      this->m_rotation = mesh.m_rotation;
      this->m_scale = mesh.m_scale;

      this->m_model_matrix = mesh.m_model_matrix;
    }
    //Accessors


    //Modifiers
    void setPosition(const glm::vec3& position) { m_position = position; }
    void setOrigin(const glm::vec3& origin) { m_origin = origin; }
    void setRotation(const glm::vec3& rotation) { m_rotation = rotation; }
    void setScale(const glm::vec3& scale) { m_scale = scale; }

    void move(const glm::vec3& position) { m_position += position; }
    void rotate(const glm::vec3& rotation) { m_rotation += rotation; }
    void scale(const glm::vec3& scale) { m_scale += scale; }

    //Functions
    void Update(const float& dt = float(1.0f))
    {
      //m_rotation.y += 0.1f * dt;
    }


    void Draw(Shader* shader)
    {
      UpdateModelMatrix();
      UpdateUniforms(shader);

      //BIND VERTEX ARRAY OBJECT (THE BIG BOX)
      glBindVertexArray(m_VAO);

      //USE THE SHADER
      shader->use();

      //Bind VAO
      glBindVertexArray(m_VAO);

      //DRAW NOTE: Blender Exports by default Quads, Remember to import obj files with option 'triangulate faces'.
      //For me it took a lot of Time to realize that blender by default
      //	exports quads instead of triangles
      if(m_nrOfIndices == 0) // if we're drawing vertices directly no index pointers, use glDrawArrays, otherwise use glDrawElements
        glDrawArrays(GL_TRIANGLES, 0, m_nrOfVertices); // <= This one draws all of the vertices
      else
        glDrawElements(GL_TRIANGLES, m_nrOfIndices, GL_UNSIGNED_INT, 0); //0: starting from 0; <= this one draws only vertices pointed to in the index buffer

      //RESET/UNBIND EVERYTHING FOR NEW FRAME
      glBindVertexArray(0);
      shader->unuse();
    }

    ~Mesh()
    {
      //Cleanup VAO VBO EBO
      glDeleteVertexArrays(1, &m_VAO);
      glDeleteBuffers(1, &m_VBO);
      if (m_nrOfIndices > 0)
        glDeleteBuffers(1, &m_EBO);
    }


};
#endif