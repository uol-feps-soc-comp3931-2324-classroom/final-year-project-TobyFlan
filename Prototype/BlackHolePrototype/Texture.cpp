#include "Texture.h"

Texture::Texture(const char* image, GLenum textureType, GLenum slot, GLenum format, GLenum pixelType) {


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
	glActiveTexture(slot);
	// Bind texture type to texture unit
	glBindTexture(textureType , ID);

	// Define how we want our textures to be handled in certain cases
	// Define how resising is handled
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // minimised
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // magnified
	// Define how texture is repeated over a space
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT); // s is for x axis
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT); // t is for y axis

	// Finally, load our texture into the texture unit
	glTexImage2D(textureType, 0, GL_RGBA, imgWidth, imgHeight, 0, format, pixelType, bytes);
	// Generate smaller versions of image
	glGenerateMipmap(textureType);

	// Free memory used by stb load
	stbi_image_free(bytes);
	// Unbind texture
	glBindTexture(textureType, 0);




}

void Texture::textureUnit(Shader& shader, const char* uniform, GLuint unit) {

	GLuint textureUniform = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(textureUniform, unit);

}

void Texture::Bind() {

	glBindTexture(type, ID);

}

void Texture::Unbind() {

	glBindTexture(type, 0);

}

void Texture::Delete() {

	glDeleteTextures(1, &ID);

}