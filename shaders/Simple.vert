#version 450 core

in vec3 position;
out vec3 pos;

uniform mat4 viewMatrix;

void main()
{
	pos = position;
    gl_Position = viewMatrix * vec4(position, 1.0);
}