#version 450 core

in vec3 fragmentWorldPos;
out vec4 outColor;

void main()
{
    outColor = vec4(fragmentWorldPos, 1.0);
}