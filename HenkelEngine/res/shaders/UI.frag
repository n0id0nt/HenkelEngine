#version 440 core
out vec4 o_Color;

in vec2 v_TexCoord;
in float v_TexIndex;
in vec4 v_Color;

uniform sampler2D U_Textures;

void main()
{
    int index = int(v_TexIndex);
    o_Color = v_Color * texture(U_Textures, v_TexCoord);
}