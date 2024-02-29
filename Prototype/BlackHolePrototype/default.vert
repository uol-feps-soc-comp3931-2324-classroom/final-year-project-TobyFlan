//initial simple shaders
#version 330 core
//version 3.3 of OpenGL


layout (location = 0 ) in vec3 aPos;
layout (location = 1 ) in vec3 aColour;
layout (location = 2 ) in vec2 aTexture;
// Outputs colours, passed in by main program
out vec3 colour;
out vec2 texCoord;

// Recieves a scale modifier for positions of objects
uniform float scale;

// Import 3d stuff into vert shader
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){

	gl_Position = projection * view * model * vec4(aPos, 0.1);
	colour = aColour;
	texCoord = aTexture;

}