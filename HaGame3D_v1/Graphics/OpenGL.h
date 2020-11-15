#ifndef OPEN_GL
#define OPEN_GL
#include <gl/glew.h>
#include <type_traits>
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {

		namespace gl {
			template <class T>
			GLenum getType() {
				if (std::is_floating_point<T>::value) return GL_FLOAT;
				else if (std::is_integral<T>::value) return GL_INT;
				else if (std::is_unsigned<T>::value) return GL_UNSIGNED_INT;
				else throw new Exception("Type not supported for OpenGL");
			}

			template <class T, size_t vectSize>
			GLuint loadVAO(const Array<float> buffer) {
				GLuint vao;
				GLuint vbo;
				GLenum type = getType<T>();

				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vbo);

				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(T) * buffer.size(), &buffer[0], GL_STATIC_DRAW);

				glBindVertexArray(vao);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, vectSize, type, GL_FALSE, vectSize * sizeof(T), (void*)0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

				return vao;
			}

			glm::mat4 get2DModelMatrix(Vec3 position, Vec3 origin, Vec3 scale, float rotation);
		}
	}
}

#endif