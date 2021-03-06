#include "Texture.h"

hagame::graphics::Texture::Texture(std::string _path) : path(_path) {
	data = stbi_load(path.c_str(), &size[0], &size[1], &channels, STBI_rgb_alpha);
	if (data == nullptr) {
		std::cout << "Failed to load texture: " << _path << std::endl;
		throw new std::exception("Failed to load texture. Check console for more details.");
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size[0], size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void hagame::graphics::Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}
