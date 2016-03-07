#version 450 core

in vec2 uv;
out vec4 outColor;

void main()
{
	// Radius is 1

	// Distance from center of sphere
	float distance = length(uv);
	if(distance > 1.0)
	{
		discard;
	}

	// Calculate normal of sphere
	float product = dot(uv,uv); // (uv.x*uv.x) + (uv.y*uv.y)
	vec3 normal = normalize(vec3(uv, sqrt(1.0 - product)));

	// Output color
    outColor = vec4(normal, 1);
}