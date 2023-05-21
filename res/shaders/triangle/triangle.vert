#version 460 core
// my vertex layout has only positions for now
layout(location = 0) in vec3 a_Position;

out vec4 vertex_color; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(a_Position, 1.0); // see how we directly give a vec3 to vec4's constructor
    //vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
}