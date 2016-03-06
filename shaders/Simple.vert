#version 450 core

in vec3 position;
out vec3 vertexWorldPos;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
	vertexWorldPos = position;
    gl_Position = projMatrix * viewMatrix * vec4(position, 1.0);
}