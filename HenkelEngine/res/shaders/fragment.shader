#version 330 core
in vec2 TexCoord;
out vec4 color;

// Texture samplers
uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
    // Linearly interpolate between both textures (second texture is only slightly combined)
    color = texture(image, TexCoord) * spriteColor;
}