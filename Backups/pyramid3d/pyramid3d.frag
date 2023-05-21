#version 460

// we expecting these outputs from vertex shader
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texture_coords;
in vec3 vs_normal;

out vec4 final_pixel_color; // Final Current Pixel Color

uniform sampler2D brick_texture; // index of the texture: example 0, 1, 2 ... shader will shader.setInt("brick_texture", 0); when texture is bound to 0

void main()
{
    final_pixel_color = texture(brick_texture, vs_texture_coords);
}