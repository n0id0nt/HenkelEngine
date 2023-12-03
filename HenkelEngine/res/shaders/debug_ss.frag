#version 330 core
out vec4 o_Color;

in vec3 v_Color;

void main()
{
    // Linearly interpolate between both textures (second texture is only slightly combined)
    o_Color = vec4(v_Color, 1.0f);;
}