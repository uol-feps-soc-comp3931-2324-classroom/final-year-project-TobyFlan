#include "shaderClass.h"


std::string get_file_contents(const char* filename) {

	// open file 
	std::ifstream in(filename, std::ios::binary);
	if(in){

		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);

	}
	// sanity check on file opening
	throw(errno);
}

Shader::Shader(const char* vertFile, const char* fragFile) {

	// read code from shader files
	std::string vertexCode = get_file_contents(vertFile);
	std::string fragmentCode = get_file_contents(fragFile).c_str();

	// convert raw code into c string format for use
	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	// create reference to shaders
	// create vertex shader object and get refrence
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach vertex shader to vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile vertex shader
	glCompileShader(vertexShader);

	// create frag shader object and get refrence
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// attach frag shader to frag shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// compile frag shader
	glCompileShader(fragmentShader);

	// wrap these up into a shader program object
	ID = glCreateProgram();
	// Attach Vertex and Frag shaders together into the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Link all shaders together into Shader Program
	glLinkProgram(ID);

	// delete old shaders now
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::Activate() {

	glUseProgram(ID);

}

void Shader::Delete() {

	glDeleteProgram(ID);

}