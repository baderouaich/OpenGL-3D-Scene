#version 460

uniform vec4 mColor; // m_shader->SetVec4f("mColor", m_triangle_color);

out vec4 final_pixel_color; // Final Current Pixel Color

void main()
{
    final_pixel_color = mColor;
}