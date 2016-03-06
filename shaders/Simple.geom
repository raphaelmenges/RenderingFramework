#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vertexWorldPos[];
out vec3 fragmentWorldPos;

void main()
{
    gl_Position = gl_in[0].gl_Position;
	fragmentWorldPos = vertexWorldPos[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
	fragmentWorldPos = vertexWorldPos[1];
    EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	fragmentWorldPos = vertexWorldPos[2];
    EmitVertex();

    EndPrimitive();
}