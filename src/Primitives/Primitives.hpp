//
// Created by bader on 5/20/23.
//
#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <Graphics/Buffer/Vertex/Vertex.hpp>

/*
*	Base Class Primitive to draw Cubes, Circles, Rects...
* http://edeleastar.github.io/opengl-programming/topic06/pdf/1.Polygons.pdf
*/
class Primitive {
private:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    bool m_dynamic; // primitive is movable ?
public:
    Primitive() {}

    virtual ~Primitive() {}

    bool isDynamic() const noexcept { return m_dynamic; }

    void Set(const Vertex *vertices, const std::size_t nrOfVertices,
             const GLuint *indices, const std::size_t nrOfIndices, bool dynamic = false) {
      for (std::size_t i = 0; i < nrOfVertices; i++) {
        m_vertices.push_back(vertices[i]);
      }
      //if no indices nrOfIndices is 0
      for (std::size_t i = 0; i < nrOfIndices; i++) {
        m_indices.push_back(indices[i]);
      }

      m_dynamic = dynamic;
    }

    const std::vector<GLuint> &GetIndices() const noexcept { return m_indices; }

    const std::vector<Vertex> &GetVertices() const noexcept { return m_vertices; }

    std::vector<GLuint> &GetIndices() noexcept { return m_indices; }

    std::vector<Vertex> &GetVertices() noexcept { return m_vertices; }

    std::size_t GetVerticesCount() const { return m_vertices.size(); }

    std::size_t GeIndicesCount() const { return m_indices.size(); }
};



/////======= PRIMITIVES =======///////
/*
* http://edeleastar.github.io/opengl-programming/topic06/pdf/1.Polygons.pdf
* Quads:
*If you add one more side to
*	a triangle, you get a
*	quadrilateral, or a foursided figure.
*OpenGL�s GL_QUADS
*	primitive draws a foursided polygon.
*This quad has a clockwise
*	winding.
*All four corners of the
*	quadrilateral must lie in a
*	plane (no bent quads).
*/
class Quad : public Primitive {
public:
    Quad()
            :
            Primitive() {
      Vertex vertices[] // square vertices
              {
                      //Position						 //Color							//Texture					//Normals
                      {glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(
                              0.0f, 0.0f, 1.0f)},
                      {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(
                              0.0f, 0.0f, 1.0f)},
                      {glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(
                              0.0f, 0.0f, 1.0f)},
                      {glm::vec3(0.5f, 0.5f, 0.0f),   glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(
                              0.0f, 0.0f, 1.0f)},
              };
      constexpr size_t nrOfVertices = sizeof(vertices) / sizeof(Vertex);

      GLuint indices[] //start with vertice point 0 , 1 then 3
              {
                      0, 1, 2, //Triangle 1
                      0, 2, 3, //Triangle 2
              };
      constexpr size_t nrOfIndices = sizeof(indices) / sizeof(GLuint);

      this->Set(vertices, nrOfVertices, indices, nrOfIndices);
    }

    virtual ~Quad() {
    }

private:

};


class Pyramid : public Primitive {
public:
    Pyramid()
            :
            Primitive() {
      std::vector<Vertex> vertices =
              {
                      //Position								//Color							//Texcoords					//Normals
                      //Triangle front
                      Vertex{glm::vec3(0.f, 0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.5f, 1.f),
                             glm::vec3(0.f, 0.f, 1.f)},
                      Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f),
                             glm::vec3(0.f, 0.f, 1.f)},
                      Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),
                             glm::vec3(0.f, 0.f, 1.f)},

                      //Triangle left
                      Vertex{glm::vec3(0.f, 0.5f, 0.f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.5f, 1.f),
                             glm::vec3(-1.f, 0.f, 0.f)},
                      Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f),
                             glm::vec3(-1.f, 0.f, 0.f)},
                      Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),
                             glm::vec3(-1.f, 0.f, 0.f)},

                      //Triangle back
                      Vertex{glm::vec3(0.f, 0.5f, 0.f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.5f, 1.f),
                             glm::vec3(0.f, 0.f, -1.f)},
                      Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f),
                             glm::vec3(0.f, 0.f, -1.f)},
                      Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),
                             glm::vec3(0.f, 0.f, -1.f)},

                      //Triangles right
                      Vertex{glm::vec3(0.f, 0.5f, 0.f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.5f, 1.f),
                             glm::vec3(1.f, 0.f, 0.f)},
                      Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f),
                             glm::vec3(1.f, 0.f, 0.f)},
                      Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),
                             glm::vec3(1.f, 0.f, 0.f)},
              };

      this->Set(vertices.data(), vertices.size(), nullptr, 0);
    }

    virtual ~Pyramid() {

    }
};