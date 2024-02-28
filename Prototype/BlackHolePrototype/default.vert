//initial simple shaders
#version 330 core
//version 3.3 of OpenGL


layout (location = 0 ) in vec3 aPos;
layout (location = 1 ) in vec3 aColour;

// Outputs colours, passed in by main program
out vec3 colour;

// Recieves a scale modifier for positions of objects
uniform float scale;

void main(){

	gl_Position = vec4(aPos.x + aPos.x*scale, aPos.y+ aPos.y*scale, aPos.z+ aPos.z*scale, 1.0);
	colour = aColour;

}