#version 450 core

in vec3 position;
out vec3 pos;

void main()
{
	pos = position;
    gl_Position = vec4(position, 1.0);
}