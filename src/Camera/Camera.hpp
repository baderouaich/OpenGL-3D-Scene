//
// Created by bader on 5/21/23.
//

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Direction
{
    FORWARD = 0,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Ref: https://learnopengl.com/Getting-started/Camera
class Camera
{
private:
    glm::mat4 m_view_matrix{};

    float m_movement_speed;
    float m_sensitivity;

    glm::vec3 m_world_up;
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;

    float m_pitch; //UP and DOWN
    float m_yaw; //LEFT and RIGHT

    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors()
    {
      // calculate the new Front vector
      m_front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
      m_front.y = std::sin(glm::radians(m_pitch));
      m_front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));

      // calculate the new Front vector
      m_front = glm::normalize(m_front);
      // also re-calculate the Right and Up vector
      m_right = glm::normalize(glm::cross(m_front, m_world_up));
      m_up = glm::normalize(glm::cross(m_right, m_front));
    }

public:
    Camera(const glm::vec3& position = glm::vec3(0.0f),
           const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f))
            :
            m_view_matrix(1.0f),
            m_world_up(worldUp),
            m_position(position),
            m_front(0.0f, 0.0f, -1.0f),
            m_right(glm::vec3(0.0f)),
            m_up(worldUp)
    {
      m_movement_speed = 10.0f;
      m_sensitivity = 10.0f;
      m_yaw = -90.0f;
      m_pitch = 0.0f;


      UpdateCameraVectors();

    }

    ~Camera()
    {

    }

    //Accessors
    const glm::mat4& GetViewMatrix()
    {
      UpdateCameraVectors();
      m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
      return m_view_matrix;
    }
    const glm::vec3& GetPosition() const { return m_position; }
    glm::vec3& GetPosition() { return m_position; }
    const glm::vec3& GetFront() const { return m_front; }
    const glm::vec3& GetWorldUp() const { return m_world_up; }



    //Functions
    void Move(float dt, Direction direction)
    {
      const float velocity = m_movement_speed * dt;
      switch (direction)
      {
        case FORWARD:
          m_position += m_front * velocity;
          break;
        case BACKWARD:
          m_position -= m_front * velocity;
          break;
        case LEFT:
          m_position -= m_right * velocity;
          break;
        case RIGHT:
          m_position += m_right * velocity;
          break;
        case UP:
          m_position.y += velocity;
          break;
        case DOWN:
          m_position.y -= velocity;
          break;
      }
    }

    void UpdateMouseInput(const float dt, float offsetX, float offsetY)
    {
      offsetX *= m_sensitivity * dt;
      offsetY *= m_sensitivity * dt;
      m_yaw   += offsetX;
      m_pitch += offsetY;

      // make sure that when pitch is out of bounds, screen doesn't get flipped
      if (m_pitch > 80.0f)
        m_pitch = 80.0f;
      if (m_pitch < -80.0f)
        m_pitch = -80.0f;

      if (this->m_yaw > 360.f || this->m_yaw < -360.f)
        this->m_yaw = 0.f;
      // update Front, Right and Up Vectors using the updated Euler angles
      UpdateCameraVectors();
      /*
      // Update m_sensitivity , m_yaw and m_roll
      m_pitch += static_cast<float>(offsetY) * m_sensitivity * dt;
      m_yaw += static_cast<float>(offsetX) * m_sensitivity * dt;

      if (this->m_pitch > 80.f)
        this->m_pitch = 80.f;
      else if (this->m_pitch < -80.f)
        this->m_pitch = -80.f;

      if (this->m_yaw > 360.f || this->m_yaw < -360.f)
        this->m_yaw = 0.f;
        */
    }

//    void Update(const float& dt, Direction direction, const double offsetX, const double offsetY)
//    {
//      UpdateMouseInput(dt, offsetX, offsetY);
//    }
};