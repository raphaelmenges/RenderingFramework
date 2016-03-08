#version 450 core

#defines

out vec3 vertColor;

uniform int resolution;

const float size = 0.25;

// Calculate 3D position from index
vec3 linearTo3D(int id)
{
    int z = id / (resolution * resolution);
    id -= (z * resolution * resolution);
    int y = id / resolution;
    int x = id % resolution;
    return vec3(x,y,z);
}

void main()
{
    // Position
    vec3 position = linearTo3D(gl_VertexID);
    position -= resolution / 2;
    position *= size;

    // Color
    vertColor = fract(vec3(
            position.x * 1.2 + position.y * 0.3 + position.z * 0.3,
            position.x * 0.2 + position.y * 0.7 + position.z * 0.8,
            position.x * 0.2 + position.y * 0.9 + position.z * 0.45));

    // Give OpenGL the position
    #ifdef DISPLACE_BY_COLOR

        gl_Position = vec4(position + vertColor, 1);

    #else

        gl_Position = vec4(position, 1);

    #endif
}
