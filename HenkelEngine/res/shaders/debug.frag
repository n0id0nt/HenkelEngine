#version 330 core
layout (location = 0) out vec4 o_Color;

void main()
{
    // Linearly interpolate between both textures (second texture is only slightly combined)
    o_Color = vec3(0.0f,1.0f,0.0f,1.0f);
}