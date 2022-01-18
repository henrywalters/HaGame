#ifndef TEXTURE
#define TEXTURE

#include <iostream>
#include <GL/glew.h>
#include "./Image.h"
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		class Texture {
		public:

			GLuint id;
			Image image;

			Texture(std::string _path, ImageType _type = ImageType::RGBA);

			void bind();
		};
	}
}

#endif