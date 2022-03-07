#ifndef TEXTURE
#define TEXTURE

#include <iostream>
#include <gl/glew.h>
#include "./Image.h"
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		class Texture {
		public:

			GLuint id;
			Ptr<Image> image;

			Texture(std::string _path, ImageType _type = ImageType::RGBA);
			Texture(Vec2Int size);

			void bind();
		};
	}
}

#endif