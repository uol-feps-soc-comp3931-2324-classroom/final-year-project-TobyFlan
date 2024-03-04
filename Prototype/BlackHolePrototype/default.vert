//initial simple shaders
#version 330 core
//version 3.3 of OpenGL


layout (location = 0 ) in vec3 aPos;
layout (location = 1 ) in vec3 aColour;
layout (location = 2 ) in vec2 aTexture;
// Outputs colours, passed in by main program
out vec3 colour;
out vec2 texCoord;

uniform mat4 camMatrix;

void main(){

	// Returns coords of all vertices
	gl_Position = camMatrix * vec4(aPos, 0.1);
	// Assigns colours to the vertices
	colour = aColour;
	// Assignes texture coords to the vertices
	texCoord = aTexture;

}