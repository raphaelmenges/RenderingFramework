#version 450 core

#defines

layout(points) in;
#ifdef TRIANGLE_IMPOSTOR
    layout(triangle_strip, max_vertices = 3) out;
#else
    layout(triangle_strip, max_vertices = 4) out;
#endif
out vec2 uv;
flat out float radius;
flat out vec3 position;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

const float size = 0.175;

void main()
{
    // Set radius and position which is for all vertices the same
    radius = size;
    position = gl_in[0].gl_Position.xyz;

    // GLSL is column-major! Get world space camera vectors
    vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]); // First row of view matrix
    vec3 cameraUp = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]); // Second row of view matrix

    // Combine matrices
    mat4 M = projMatrix * viewMatrix;

    #ifdef TRIANGLE_IMPOSTOR

        gl_Position = M * (gl_in[0].gl_Position + size * vec4(-2 * cameraRight - cameraUp, 0));
        uv = 2 * (vec2(-0.5, 0) - 0.5);
        EmitVertex();

        gl_Position =  M * (gl_in[0].gl_Position + size * vec4(2 * cameraRight - cameraUp, 0));
        uv = 2 * (vec2(1.5, 0) - 0.5);
        EmitVertex();

        gl_Position = M * (gl_in[0].gl_Position + size * vec4(3 * cameraUp, 0));
        uv = 2 * (vec2(0.5, 2) - 0.5);
        EmitVertex();

        EndPrimitive();

    #else

        gl_Position = M * (gl_in[0].gl_Position + size * vec4(cameraRight+cameraUp, 0));
        uv = vec2(1,1);
        EmitVertex();

        gl_Position = M * (gl_in[0].gl_Position + size * vec4(-cameraRight+cameraUp, 0));
        uv = vec2(-1,1);
        EmitVertex();

        gl_Position = M * (gl_in[0].gl_Position + size * vec4(cameraRight-cameraUp, 0));
        uv = vec2(1,-1);
        EmitVertex();

        gl_Position = M * (gl_in[0].gl_Position + size * vec4(-cameraRight-cameraUp, 0));
        uv = vec2(-1,-1);
        EmitVertex();

        EndPrimitive();

    #endif
}
