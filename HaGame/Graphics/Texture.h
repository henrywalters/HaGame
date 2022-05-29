#ifndef TEXTURE
#define TEXTURE

#include <iostream>
#include <gl/glew.h>
#include "./Image.h"
#include "./RawTexture.h"
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		class Texture : public RawTexture<GL_RGBA8, GL_UNSIGNED_BYTE> {
		public:
			Ptr<Image> image;

			Texture(std::string _path, ImageType _type = ImageType::RGBA);
			Texture(Vec2Int size);
		};
	}
}

#endif