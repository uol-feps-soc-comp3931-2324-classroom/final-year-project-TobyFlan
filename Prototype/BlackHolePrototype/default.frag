#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs from the vertex shader
in vec3 crntPos; // Current position in world space



// Stuff for grav lensing
vec3 blackHolePos = vec3(0.0, 0.0, 0.0);

// using equation 2GM/c^2
const float schwarzschildRad = 1500.0 * 5000.0;

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


       // Adjust rayDir based on bending angle
       rayDir += -1.5 * h2 * rayPoint / pow(pow(dist, 2.0), 2.5);



        float dynamicStepSize = max(stepSize * clamp(1.0 / (0.1 * distanceToBlackHole + 1.0), 0.05, 0.5), 0.05);
        vec3 prePoint = rayPoint;
        rayPoint += dynamicStepSize * rayDir;
        traveledDistance += dynamicStepSize;





        vec3 L = sphereCenter - rayPoint;
        float tca = dot(L, rayDir);
        float d2 = dot(L, L) - tca * tca;
        float radius2 = sphereRadius * sphereRadius;

        if (d2 < radius2) {
            float thc = sqrt(radius2 - d2);
            float t0 = tca - thc;
            // Calculate exit intersection
            float t1 = tca + thc; 

             // Ensure t0 is non-negative
            if (t0 < 0) {
                t0 = max(t0, 0);
            }

            // Check if intersection is within the current step
            if (t1 < stepSize) { 
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