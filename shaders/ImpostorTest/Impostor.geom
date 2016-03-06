#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
	float delta = 0.5;

	mat4 M = projMatrix * viewMatrix;

    gl_Position = M * (gl_in[0].gl_Position + vec4(-delta, -delta, 0, 0));
    EmitVertex();

    gl_Position = M * (gl_in[0].gl_Position + vec4(+delta, -delta, 0, 0));
    EmitVertex();

	gl_Position = M * (gl_in[0].gl_Position + vec4(0, delta, 0, 0));
    EmitVertex();

    EndPrimitive();
}