#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "./Image.h"
#include <gl/glew.h>
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		class Cubemap {
		public:
			GLuint id;
			Array<Ptr<Image>> images;

			Cubemap(String paths[6], ImageType imageType = ImageType::RGB)
			{
				images = Array<Ptr<Image>>();
				glGenTextures(1, &id);
				bind();

				for (int i = 0; i < 6; i++) {

					images.push_back(std::make_shared<Image>(paths[i], imageType));

					glTexImage2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
						0, GL_RGB, images[i]->size[0], images[i]->size[1], 0, GL_RGB, GL_UNSIGNED_BYTE, images[i]->data
					);
				}

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			}

			void bind() {
				glBindTexture(GL_TEXTURE_CUBE_MAP, id);
			}
		};
	}
}

#endif
