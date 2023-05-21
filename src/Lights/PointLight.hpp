//
// Created by bader on 5/21/23.
//
#pragma once

#include "Light.hpp"

// https://learnopengl.com/Lighting/Basic-Lighting
class PointLight : public Light
{
protected:
    glm::vec3 m_position;
    float m_constant;
    float m_linear;
    float m_quadratic;

public:
    PointLight(const glm::vec3& position,
               float intensity,
               const glm::vec3& color = glm::vec3(1.0f),
               float constant = 1.0f,
               float linear = 0.045f,
               float quadratic = 0.0075f)
            :
            Light(intensity, color),
            m_position(position),
            m_constant(constant),
            m_linear(linear),
            m_quadratic(quadratic)
    {
    }

    const glm::vec3& GetPosition() const { return m_position; }
    glm::vec3& GetPosition() { return m_position; }
    void SetPosition(const glm::vec3& position) { m_position = position; }


    void SendToShader(const std::shared_ptr<Shader>& shader) override
    {
      shader->SetFloat("pointLight.intensity", m_intensity);
      shader->SetVec3f("pointLight.position", m_position);
      shader->SetVec3f("pointLight.color", m_color);
      shader->SetFloat("pointLight.constant", m_constant);
      shader->SetFloat("pointLight.linear", m_linear);
      shader->SetFloat("pointLight.quadratic", m_quadratic);
    }

    ~PointLight() override = default;


};