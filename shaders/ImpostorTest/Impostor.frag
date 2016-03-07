#version 450 core

in vec2 uv;
flat in float radius;
flat in vec3 position;
out vec4 outColor;
layout (depth_less) out float gl_FragDepth; // Makes optimizations possible

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
	// Radius in UV space is 1

	// Distance from center of sphere
	float distance = length(uv);
	if(distance > 1.0)
	{
		discard;
	}

	// Calculate normal of sphere
	float z = sqrt(1.0 - dot(uv,uv)); // 1.0 -((uv.x*uv.x) + (uv.y*uv.y)));
	vec3 normal = normalize(vec3(uv, z));

	// World space position on sphere
	vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]); // First row of view matrix
	vec3 cameraUp = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]); // Second row of view matrix
	vec3 cameraDepth = vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]); // Third row of view matrix
	vec3 relativeViewPos = normal * radius;
	vec3 relativeWorldPos = vec3(relativeViewPos.x * cameraRight + relativeViewPos.y * cameraUp + relativeViewPos.z * cameraDepth);

	// Set depth of pixel by projecting pixel position into clip space (TODO: DO NOT USE VIEW SPACE NORMAL, MUST USE WORLD SPACE)
	vec4 projPos = projMatrix * viewMatrix * vec4(position + relativeWorldPos, 1.0);
	float projDepth = projPos.z / projPos.w;
	gl_FragDepth = (projDepth + 1.0) * 0.5; // gl_FragCoord.z is from 0..1. So go from clip space to viewport space

	// Output color
    outColor = vec4(normal, 1);
}