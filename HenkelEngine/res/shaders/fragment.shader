#version 330 core
in vec2 TexCoord;

out vec4 color;

// Texture samplers
uniform sampler2D ourTexture1;
uniform vec3 faceColor;

void main()
{
    // Linearly interpolate between both textures (second texture is only slightly combined)
    color = texture(ourTexture1, TexCoord) * vec4(faceColor, 1.0f);
}