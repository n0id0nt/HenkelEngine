#version 330 core
layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;

// Texture samplers
uniform sampler2D u_Texture;

void main()
{
    vec2 size = textureSize(u_Texture, 0);

    vec2 uv = v_TexCoord * size;
    vec2 duv = fwidth(v_TexCoord);
    uv = floor(uv) + vec2(0.5) + clamp((fract(uv) - vec2(0.5) + duv)/duv, vec2(0,0), vec2(1,1));
    uv /= size;

    o_Color = texture(u_Texture, uv) * v_Color;
}