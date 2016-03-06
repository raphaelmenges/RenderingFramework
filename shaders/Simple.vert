#version 450 core

in vec3 position;
out vec3 pos;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
	pos = position;
    gl_Position = projMatrix * viewMatrix * vec4(position, 1.0);
}