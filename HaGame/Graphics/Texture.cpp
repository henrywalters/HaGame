#include "Texture.h"

hagame::graphics::Texture::Texture(std::string path, ImageType type): 
	image(std::make_shared<Image>(path, type))
{
	initialize(image->size, image->data);

	setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);
}

hagame::graphics::Texture::Texture(Vec2Int size): 
	image(std::make_shared<Image>()),
	RawTexture<GL_RGBA8, GL_UNSIGNED_BYTE>(size)
{

}