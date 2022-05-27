#ifndef RAW_TEXTURE_H
#define RAW_TEXTURE_H

#include <gl/glew.h>
#include "./Image.h"
#include "../Utils/Aliases.h"


namespace hagame {
	namespace graphics {
		
		// The RawTexture class provides a thin wrapper around OpenGL textures.
		template <GLuint TextureType>
		class RawTexture {
		public:

			GLuint id;

			RawTexture(Vec2Int size) {
				glGenTextures(1, &id);
				glBindTexture(GL_TEXTURE_2D, id);
				glTexImage2D(GL_TEXTURE_2D, 0, TextureType, size[0], size[1], 0, GL_RGBA, GL_FLOAT, NULL);
				setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}

			void setParameter(GLuint param, GLuint value) {
				glTexParameteri(GL_TEXTURE_2D, param, value);
			}

			void bind() {
				glBindTexture(GL_TEXTURE_2D, id);
			}
		};
	}
}

#endif
