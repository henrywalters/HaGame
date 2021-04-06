#ifndef TEXTURE
#define TEXTURE

#include <iostream>
#include <gl/glew.h>
#include "../Vendor/stb/stb_image.h"
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		class Texture {
		public:
			GLuint id;
			Vec2Int size;
			int channels;
			std::string path;
			unsigned char* data;

			Texture(std::string _path);

			void bind();
		};
	}
}

#endif