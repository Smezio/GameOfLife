#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform mat4 model;
uniform mat4 proj;

void main()
{
    gl_Position = proj * model * vec4(aPos, 1.0);
    vertexColor = vec4(0.5, 1.0, 0.6, 1.0);
}