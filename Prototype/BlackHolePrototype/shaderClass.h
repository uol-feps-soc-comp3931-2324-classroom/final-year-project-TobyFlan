#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<cerrno>
#include<fstream>
#include<sstream>
#include<iostream>


std::string get_file_contents(const char* filename);

class Shader {

public:
	GLuint ID;
	// Constructor builds shader program from both vert and frag files
	Shader(const char* vertFile, const char* fragFile);

	// Enable the current shader program
	void Activate();
	// Delete the shader program
	void Delete();

};


#endif
