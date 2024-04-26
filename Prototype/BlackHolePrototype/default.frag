#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs from the vertex shader
in vec3 crntPos; // Current position in world space


vec3 blackHolePos = vec3(0.0, 0.0, 0.0);
const float G = 6.674010551359e-11;  // Reduced gravitational constant for visible effect
const float M = 1e31;  // Reduced mass for testing
const float c = 299792458;  // Speed of light in vacuum, m/s
const float eventHorizon = 9.0;
const float pi = 3.1415926535897932384626;


// Uniforms
uniform sampler2D diffuse0; // Texture of the sphere
uniform vec3 sphereCenter;
uniform float sphereRadius;
uniform vec3 camPos; // Camera position in world space

const float stepSize = 10.0; // Step size for ray marching
const float maxDistance = 10000.0; // Maximum distance to trace the ray (assuming double the sphere radius for safety)

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

    vec4 returnColor = vec4(0.0, 0.0, 0.0, 1.0);

        
    // Calculate schwarzschild radius to render event horizon
    float schwarzschildRad = (2 * G * M) / (c * c);

    // Im losing my mind but this seems like its a reasonable size.
    schwarzschildRad = schwarzschildRad  * 160;



    while (traveledDistance < maxDistance) {

        float distanceToBlackHole = length(blackHolePos - rayPoint) * 1e6; // Convert distance to meters
        
        // Calculate gravitational bending angle
        float angle = 4.0 * G * M / (distanceToBlackHole * c * c);

        // Adjust rayDir based on bending angle
        vec3 toBlackHoleNormalized = normalize(blackHolePos - rayPoint);
        rayDir = normalize(rayDir + angle * toBlackHoleNormalized);


        if (distanceToBlackHole < schwarzschildRad) {
            FragColor = vec4(0, 0, 0, 1);  // Render black inside event horizon
        return;
        }

        float dynamicStepSize = stepSize * clamp(1.0 / (0.1 * distanceToBlackHole + 1.0), 0.1, 1.0);
        rayPoint += dynamicStepSize * rayDir;
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
                vec2 hitTexCoords = vec2(0.5 - atan(normal.z, normal.x) / (2.0 * pi),
                                         0.5 + asin(normal.y) / pi);
                vec4 texColor = texture(diffuse0, hitTexCoords);
                returnColor = texColor;


            }
        }
    }

    FragColor = returnColor;

}