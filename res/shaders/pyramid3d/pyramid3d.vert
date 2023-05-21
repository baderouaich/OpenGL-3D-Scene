#version 460 core

// Vertex stuff
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture_coords;
layout (location = 3) in vec3 normal;

//we will be sending these output to fragment
out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texture_coords;
out vec3 vs_normal;

// Matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_position = vec4(model * vec4(position, 1.0f)).xyz;
    vs_color = color;

    //* -1 is to flip texture coordinates (use it here or by stb_image_flip before image loaded)
    vs_texture_coords = vec2(texture_coords.x, texture_coords.y/* * -1.0f*/);

    //Normal
    vs_normal = mat3(model) * normal;

    //final position of vertex
    gl_Position = projection * view * model * vec4(position, 1.0f);
}