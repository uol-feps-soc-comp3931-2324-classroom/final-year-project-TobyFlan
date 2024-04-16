#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs from the vertex shader
in vec3 crntPos; // Current position in world space
in vec2 texCoord; // Texture coordinates

// Uniforms
uniform sampler2D diffuse0; // Texture of the sphere
uniform mat4 invViewMatrix; // Inverse of the view matrix (Camera matrix)
uniform vec3 sphereCenter;
uniform float sphereRadius;
uniform vec3 camPos; // Camera position in world space

void main() {
    // Compute the ray direction
    vec3 rayOrigin = camPos;
    vec3 rayDir = normalize(crntPos - camPos);

    // Sphere-ray intersection test
    vec3 oc = rayOrigin - sphereCenter;
    float b = dot(oc, rayDir);
    float c = dot(oc, oc) - sphereRadius * sphereRadius;
    float h = b * b - c;
    if (h < 0.0) {
        FragColor = vec4(1, 0, 0, 1); // Red means no intersection
        return;
    } 
    h = sqrt(h);
    float t = -b - h; // Only the nearest intersection point

    // Compute the texture coordinates based on the intersection point
    vec3 hitPoint = rayOrigin + t * rayDir;
    vec3 normal = normalize(hitPoint - sphereCenter);
    vec2 hitTexCoords;
    hitTexCoords.x = 0.5 + atan(normal.z, normal.x) / (2.0 * 3.14159265);
    hitTexCoords.y = 0.5 - asin(normal.y) / 3.14159265;

    // Apply the texture
    vec4 texColor = texture(diffuse0, hitTexCoords);
    FragColor = texColor;
}
