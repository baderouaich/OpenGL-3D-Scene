//
// Created by bader on 5/21/23.
//
#pragma once

#include <memory>
#include "glm/vec3.hpp"
#include "Graphics/Shader/Shader.hpp"

// https://learnopengl.com/Lighting/Basic-Lighting
class Light
{
protected:
    float m_intensity;
    glm::vec3 m_color;

public:
    Light(float intensity, const glm::vec3& color)
            :
            m_intensity(intensity),
            m_color(color)
    {
    }

    void SetIntensity(float intensity) { m_intensity = intensity; }
    void SetColor(const glm::vec3& color) { m_color = color; }

    float GetIntensity() const { return m_intensity; }
    const glm::vec3& GetColor() const { return m_color; }

    virtual void SendToShader(const std::shared_ptr<Shader>& shader) = 0;


    virtual ~Light() = default;


};