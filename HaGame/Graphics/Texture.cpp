#include "Texture.h"

hagame::graphics::Texture::Texture(std::string path, ImageType type): image(std::make_shared<Image>(path, type)) {

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->size[0], image->size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
}

hagame::graphics::Texture::Texture(Vec2Int size)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// Might need to change to GL_RGB
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size[0], size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Might need to change to GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void hagame::graphics::Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}
