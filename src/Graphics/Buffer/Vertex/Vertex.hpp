#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texture_coords;
    glm::vec3 normal; // points to direction (where light where shine to)
};