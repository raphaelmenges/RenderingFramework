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

	// Calculate normal of sphere (stupid human has mistake)
	float x = uv.x;
	float y = uv.y;
	float z = cos(atan(uv.y, uv.x));
	vec3 normal = normalize(vec3(x,y, z));

    outColor = vec4(normal, 1);
}