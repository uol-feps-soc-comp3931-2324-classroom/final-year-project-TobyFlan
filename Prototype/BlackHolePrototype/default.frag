#version 330 core


out vec4 FragColour;

// Takes input as colour from .vert shader and returns it.
in vec3 colour;

void main(){

	FragColour = vec4(colour, 1.f);

}