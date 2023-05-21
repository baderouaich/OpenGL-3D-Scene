//
// Created by bader on 5/21/23.
//
#pragma once

#include <Graphics/Texture/Texture.hpp>
#include <Model/Mesh.hpp>
#include "Material.hpp"
#include "OBJLoader.hpp"

// 3D Model (.obj)
class Model {
private:
    std::shared_ptr<Material> m_material;
    std::shared_ptr<Texture> m_override_texture_diffuse;
    std::shared_ptr<Texture> m_override_texture_specular;
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    glm::vec3 m_position;

public:
    Model(const glm::vec3& position,
          const std::shared_ptr<Material>& material,
          const std::shared_ptr<Texture>& texture_diffuse,
          const std::shared_ptr<Texture>& texture_specular)
            :
            m_material(material),
            m_override_texture_diffuse(texture_diffuse),
            m_override_texture_specular(texture_specular),
            m_position(position)
    {


    }

    /// OBJ File Loaded Model
    Model(
            const glm::vec3& position,
            const std::shared_ptr<Material>& material,
            const std::shared_ptr<Texture>& texture_diffuse,
            const std::shared_ptr<Texture>& texture_specular,
            const std::string& obj_filename)
            :
            m_material(material),
            m_override_texture_diffuse(texture_diffuse),
            m_override_texture_specular(texture_specular),
            m_position(position)
    {

      std::vector<Vertex> vertices = OBJLoader::LoadOBJ(obj_filename);

      m_meshes.emplace_back(new Mesh(
              vertices,
              std::vector<GLuint>(), // no indices, draw all vertices from 0 to N
              position,
              glm::vec3(0.0f),
              glm::vec3(0.0f),
              glm::vec3(1.0f)
      ));

      //Move each mesh to position of model
      for (const auto& mesh : m_meshes)
      {
        mesh->setOrigin(m_position);
        mesh->move(m_position);
      }
    }


    //Functions
    void rotate(const glm::vec3& rotation)
    {
      for (const auto& mesh : m_meshes)
      {
        mesh->rotate(rotation);
      }
    }

    void Update(float dt)
    {
      for (const auto& mesh : m_meshes)
      {
//        mesh->setPosition(m_position);
        mesh->Update(dt);
      }
    }
    void Draw(const std::shared_ptr<Shader>& shader)
    {
      //Update Uniforms
      //      UpdateUniforms(shader);

      //USE A SHADER PROGRAM FOR DRAWING (not uniforms)  Use a program (BECAUSE SHADER CLASS LAST UNIFORM UPDATE UNUSES IT)
      shader->Bind();

      //MATERIALS (TODO: when material is dirty then send to shader not every frame.)
      m_material->SendToShader(shader);


      /// == DRAW == ///
      //DRAW MESHES
      for (const auto& mesh : m_meshes)
      {
        //ACTIVATE A TEXTURE
        m_override_texture_diffuse->Bind(0);
        m_override_texture_specular->Bind(1);

        mesh->Draw(shader);
      }


    }


    ~Model() = default;

public:

    const glm::vec3 &GetPosition() const {
      return m_position;
    }
    glm::vec3 &GetPosition() {
      return m_position;
    }
    void SetPosition(const glm::vec3 &mPosition) {
      m_position = mPosition;
    }

    const std::vector<std::shared_ptr<Mesh>> &GetMeshes() const {
      return m_meshes;
    }

public:
    void AddMesh(const std::shared_ptr<Mesh>& mesh){
      m_meshes.push_back(mesh);
      // Move each mesh to position of model
      m_meshes.back()->move(m_position);
      m_meshes.back()->setOrigin(m_position);
    }
};
