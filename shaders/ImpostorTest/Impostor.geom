#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;
out vec2 uv;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
	float delta = 0.5;
	mat4 M = projMatrix * viewMatrix;

    gl_Position = M * (gl_in[0].gl_Position + vec4(-delta, -delta, 0, 0));
	uv = 2 * vec2(-0.5, -0.5);
    EmitVertex();

    gl_Position = M * (gl_in[0].gl_Position + vec4(3 * delta, -delta, 0, 0));
	uv = 2 * vec2(1.5, -0.5);
    EmitVertex();

	gl_Position = M * (gl_in[0].gl_Position + vec4(-delta, 3 * delta, 0, 0));
	uv = 2 * vec2(-0.5, 1.5);
    EmitVertex();

    EndPrimitive();
}