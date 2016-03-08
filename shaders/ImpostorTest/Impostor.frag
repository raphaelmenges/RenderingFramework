#version 450 core

#defines

in vec2 uv;
flat in float radius;
flat in vec3 position;
out vec4 outColor;
layout (depth_less) out float gl_FragDepth; // Makes optimizations possible

uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec3 cameraWorldPos;

const vec4 lightDirection = vec4(-0.5, -0.75, -0.3, 0);

void main()
{
    #ifdef RENDER_SPHERE

        // Radius in UV space is 1 (therefore the scaling with 2 in geometry shader)

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
        vec3 worldNormal = normalize(relativeWorldPos);
        vec3 worldPos = position + relativeWorldPos;

        // Set depth of pixel by projecting pixel position into clip space
        vec4 projPos = projMatrix * viewMatrix * vec4(worldPos, 1.0);
        float projDepth = projPos.z / projPos.w;
        gl_FragDepth = (projDepth + 1.0) * 0.5; // gl_FragCoord.z is from 0..1. So go from clip space to viewport space

        // Diffuse lighting (hacked together, not correct)
        vec4 nrmLightDirection = normalize(lightDirection);
        float lighting = max(0,dot(normal, vec3(viewMatrix * -nrmLightDirection))); // Do it in view space (therefore is normal here ok)

        // Specular lighting (camera pos in view matrix last column is in view coordinates?)
        vec3 reflectionVector = reflect(nrmLightDirection.xyz, worldNormal);
        vec3 surfaceToCamera = normalize(cameraWorldPos - worldPos);
        float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
        float specular = pow(cosAngle, 25);
        lighting += lighting * 0.75 * specular;

        // Some "ambient" lighting
        vec3 color = mix(vec3(0.1, 0.2, 0.3), vec3(0.8, 0.8, 0.8), lighting);

        // Output color
        outColor = vec4(color, 1);

    #else

        outColor = vec4(1,0,0,1);
        gl_FragDepth = gl_FragCoord.z;

    #endif
}
