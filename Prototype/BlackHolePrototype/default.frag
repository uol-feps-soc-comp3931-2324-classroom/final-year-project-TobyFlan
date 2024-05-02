#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs from the vertex shader
in vec3 crntPos; // Current position in world space



// Stuff for grav lensing
vec3 blackHolePos = vec3(0.0, 0.0, 0.0);
const float G = 6.674010551359e-11;  // Reduced gravitational constant for visible effect
const float M = 1e31;  // Reduced mass for testing
const float c = 299792458;  // Speed of light in vacuum, m/s

// using equation 2GM/c^2
const float schwarzschildRad = 1500.0 * 5000.0;
const float eventHorizon = 9.0;
const float pi = 3.1415926535897932384626;

// Stuff for accretion disc
const float diskInnerRadius = 8.0;  // inner radius of the accretion disk
const float diskOuterRadius = 30.0;  // outer radius of the accretion disk


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

    float h2 = pow(length(cross(rayPoint, rayDir)), 2.0);

    while (traveledDistance < maxDistance) {

        float distanceToBlackHole = length(blackHolePos - rayPoint) * 1e6; // Convert distance to meters
        float dist = length(blackHolePos - rayPoint);

        // Return black if too close to the center of black hole
        if(dist < 7.0){
            FragColor = returnColor;
            return;
        }


        // Calculate gravitational bending angle
        //float angle = 4.0 * G * M / (distanceToBlackHole * c * c);

        // Adjust rayDir based on bending angle
        //vec3 toBlackHoleNormalized = normalize(blackHolePos - rayPoint);
       // rayDir = normalize(rayDir + angle * toBlackHoleNormalized);
       rayDir += -1.5 * h2 * rayPoint / pow(pow(dist, 2.0), 2.5);




      /*  if (distanceToBlackHole < schwarzschildRad) {
            FragColor = returnColor;
        return;
        }*/

        float dynamicStepSize = max(stepSize * clamp(1.0 / (0.1 * distanceToBlackHole + 1.0), 0.05, 0.5), 0.05);
        vec3 prePoint = rayPoint;
        rayPoint += dynamicStepSize * rayDir;
        traveledDistance += dynamicStepSize;


        // Stuff for accretion disk     
        if (dist > diskInnerRadius && dist < diskOuterRadius && prePoint.y * rayPoint.y < pow(dynamicStepSize - 1.0, 2.0)) {

            float diskDensity = 1.0 - length(rayPoint / vec3(diskOuterRadius, 1.0, diskOuterRadius));
            diskDensity *= smoothstep(diskInnerRadius, diskInnerRadius + 1.0, dist);
            diskDensity *= inversesqrt(dist);
            float opticalDepth = dynamicStepSize * 50.0 * diskDensity;
            vec3 shiftVector = 0.6 * cross(normalize(rayPoint), vec3(0.0, 1.0, 0.0));
            float velocity = dot(rayDir, shiftVector);
            float dopplerShift = sqrt((1.0 - velocity) / (1.0 + velocity));
            float gravitationalShift = sqrt((1.0 - 2.0 / dist) / (1.0 - 2.0 / length(camPos)));
            FragColor = vec4(vec3(1.0, 0.682, 0.365) * dopplerShift * gravitationalShift * opticalDepth, 1.0);
            return;
        }


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

            if (t1 < stepSize) { // Check if intersection is within the current step
                rayPoint = rayOrigin + (traveledDistance + t1) * rayDir;
                vec3 normal = normalize(rayPoint - sphereCenter);
                vec2 hitTexCoords = vec2(0.5 - atan(normal.z, normal.x) / (2.0 * pi),
                                         0.5 + asin(normal.y) / pi);
                vec4 texColor = texture(diffuse0, hitTexCoords);
                returnColor = texColor;
                break;


            }
        }
    }

    FragColor = returnColor;

}