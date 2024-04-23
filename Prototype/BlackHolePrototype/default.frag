#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs from the vertex shader
in vec3 crntPos; // Current position in world space


vec3 blackHolePos = vec3(0.0, 0.0, 0.0);
const float G = 6e-11;  // Reduced gravitational constant for visible effect
const float blackHoleMass = 1e30;  // Reduced mass for testing
const float c = 299792458;  // Speed of light in vacuum, m/s
const float eventHorizon = 8.0;

// Uniforms
uniform sampler2D diffuse0; // Texture of the sphere
uniform vec3 sphereCenter;
uniform float sphereRadius;
uniform vec3 camPos; // Camera position in world space

const float stepSize = 10.0; // Step size for ray marching
const float maxDistance = 20000.0; // Maximum distance to trace the ray (assuming double the sphere radius for safety)

void swap(float a, float b) {
    float temp = a;
    a = b;
    b = temp;
}


void main() {
    vec3 rayOrigin = camPos;
    vec3 rayDir = normalize(crntPos - camPos);
    vec3 rayPoint = rayOrigin;
    float traveledDistance = 0.0;

    vec4 returnColor = vec4(0.0, 0.0, 0.0, 0.0);

    // Using integrated schwarzschild metric
    // Eulders method constant h2
    float h2 = pow(length(cross(rayOrigin, rayDir)), 2.0);


    while (traveledDistance < maxDistance) {

        float distanceToBlackHole = length(blackHolePos - rayPoint);
        if (distanceToBlackHole > sphereRadius * 10) break;  // Early exit if far away
        // Dynapic step size that gets smaller when close to black hole
        float dynamicStepSize = stepSize * clamp(10.0 / distanceToBlackHole, 0.1, 1.0);


        rayDir += -1.5 * h2 * rayPoint / pow(pow(distanceToBlackHole, 2.0), 2.5);

        rayPoint += dynamicStepSize * rayDir;


        if (distanceToBlackHole < eventHorizon) {
            FragColor = vec4(0, 0, 0, 1);  // Black hole event horizon
            return;
        }


        traveledDistance += dynamicStepSize;

        vec3 L = sphereCenter - rayPoint;
        float tca = dot(L, rayDir);
        float d2 = dot(L, L) - tca * tca;
        float radius2 = sphereRadius * sphereRadius;

        if (d2 < radius2) {
            float thc = sqrt(radius2 - d2);
            float t0 = tca - thc;
            float t1 = tca + thc; // Calculate exit intersection

            if (t0 < 0) {
                t0 = max(t0, 0); // Ensure t0 is non-negative
            }

            if (t0 < stepSize) { // Check if intersection is within the current step
                rayPoint = rayOrigin + (traveledDistance + t0) * rayDir;
                vec3 normal = normalize(rayPoint - sphereCenter);
                vec2 hitTexCoords = vec2(0.5 - atan(normal.z, normal.x) / (2.0 * 3.14159265),
                                         0.5 + asin(normal.y) / 3.14159265);
                vec4 texColor = texture(diffuse0, hitTexCoords);
                returnColor = texColor;
               //return;
            }
        }
    }

    FragColor = returnColor;

}