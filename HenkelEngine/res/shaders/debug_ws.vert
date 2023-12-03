#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;

uniform mat4 U_ViewProjection;

out vec3 v_Color;

void main()
{
    v_Color = a_Color;
    gl_Position = U_ViewProjection * vec4(a_Position, 1.0f);
}