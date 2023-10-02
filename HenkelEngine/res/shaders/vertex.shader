#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;


uniform mat4 U_Model;
uniform mat4 U_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_Color;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    gl_Position = U_Model * U_ViewProjection * vec4(a_Position, 1.0f);
}