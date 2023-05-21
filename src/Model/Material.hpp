//
// Created by bader on 5/21/23.
//

#pragma once
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

// Material information for a model like model colors and diffuse/specular maps
class Material
{
private:
    // Same struct attributes as in fragment shader
    // Colors
    glm::vec3 m_ambient;	// Color of Ambient light means the light that is already present in a scene, before any additional lighting is added. It usually refers to natural light, either outdoors or coming through windows etc. It can also mean artificial lights such as normal room lights.
    glm::vec3 m_diffuse;	// Color of Diffused light is a soft light with neither the intensity nor the glare of direct light. It is scattered and comes from all directions. Thus, it seems to wrap around objects. It is softer and does not cast harsh shadows
    glm::vec3 m_specular; // How big is the light circle, Specular means that light is perfectly reflected in a mirror-like way from the light source to the viewer.
    GLint m_diffuse_texture;   // real color data
    GLint m_specular_texture;  // real color data

public:
    Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, GLint diffuse_texture, GLint specular_texture)
            :
            m_ambient(ambient),
            m_diffuse(diffuse),
            m_specular(specular),
            m_diffuse_texture(diffuse_texture),
            m_specular_texture(specular_texture)
    {
    }

    ~Material() = default;

public:
    const glm::vec3 &getAmbient() const {
      return m_ambient;
    }

    void setAmbient(const glm::vec3 &mAmbient) {
      m_ambient = mAmbient;
    }

    const glm::vec3 &getDiffuse() const {
      return m_diffuse;
    }

    void setDiffuse(const glm::vec3 &mDiffuse) {
      m_diffuse = mDiffuse;
    }

    const glm::vec3 &getSpecular() const {
      return m_specular;
    }

    void setSpecular(const glm::vec3 &mSpecular) {
      m_specular = mSpecular;
    }

    GLint getDiffuseTexture() const {
      return m_diffuse_texture;
    }

    void setDiffuseTexture(GLint mDiffuseTexture) {
      m_diffuse_texture = mDiffuseTexture;
    }

    GLint getSpecularTexture() const {
      return m_specular_texture;
    }

    void setSpecularTexture(GLint mSpecularTexture) {
      m_specular_texture = mSpecularTexture;
    }

public:
    void SendToShader(const std::shared_ptr<Shader>& shader) const
    {
      shader->SetVec3f("material.ambient", m_ambient);
      shader->SetVec3f("material.diffuse", m_diffuse);
      shader->SetVec3f("material.specular", m_specular);
      shader->SetInteger("material.diffuse_texture", m_diffuse_texture);
      shader->SetInteger("material.specular_texture", m_specular_texture);
    }

};