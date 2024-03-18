#version 330 core
out vec4 o_Color;

in vec2 v_TexCoord;
in int v_TexIndex;
in vec4 v_Color;

uniform sampler2D U_Textures[10];

void main()
{
    int index = int(v_TexIndex)
    o_Color = v_Color * texture(U_Textures[index], v_TexCoord);
}