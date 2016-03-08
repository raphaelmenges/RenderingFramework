#version 450 core

#defines

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
    vec3 position = linearTo3D(gl_VertexID);
    position -= resolution / 2;
    position *= size;
    gl_Position = vec4(position, 1);
}
