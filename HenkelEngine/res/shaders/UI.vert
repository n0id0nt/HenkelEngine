#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

uniform mat4 U_ViewProjection;
uniform mat4 U_Model;

out vec4 v_Color;

void main()
{
    v_Color = a_Color;
    gl_Position = U_ViewProjection * U_Model * vec4(a_Position, 1.0f);
}