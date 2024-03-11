#include "Texture.h"

Texture::Texture(const char* image, const char* textureType, GLenum slot) {


	type = textureType;
	// Define variables for image dimensions, and number of colour channels.
	int imgWidth, imgHeight, numColourChannels;

	// Flip stbi image as openGL and stbi read images in opposite directions
	stbi_set_flip_vertically_on_load(true);

	// Use stb library to load texture
	unsigned char* bytes = stbi_load(image, &imgWidth, &imgHeight, &numColourChannels, 0);

	// Tell OpenGL to generate texture object
	glGenTextures(1, &ID);
	// Activate texture unit to hold textures for shader files
	glActiveTexture(GL_TEXTURE_2D + slot);
	unit = slot;
	// Bind texture type to texture unit
	glBindTexture(GL_TEXTURE_2D, ID);

	// Define how we want our textures to be handled in certain cases
	// Define how resising is handled
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); // minimised
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // magnified
	// Define how texture is repeated over a space
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // s is for x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // t is for y axis


	// Check colour channels to identify how to assign texture
	if (numColourChannels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	}
	else if (numColourChannels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	}
	else if (numColourChannels == 1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
	}
	else throw std::invalid_argument("Error in texture.cpp; invalid number of colour channels\n");
	
	// Generate smaller versions of image
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free memory used by stb load
	stbi_image_free(bytes);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);




}

void Texture::textureUnit(Shader& shader, const char* uniform, GLuint unit) {

	GLuint textureUniform = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(textureUniform, unit);

}

void Texture::Bind() {

	glBindTexture(GL_TEXTURE_2D, ID);

}

void Texture::Unbind() {

	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::Delete() {

	glDeleteTextures(1, &ID);

}