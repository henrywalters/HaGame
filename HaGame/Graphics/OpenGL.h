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

			struct VertexBufferArray {
				GLuint vao;
				GLuint vbo;
			};

			template <class T, size_t vectSize>
			VertexBufferArray loadVBA(const Array<float> buffer) {
				VertexBufferArray data;
				GLenum type = getType<T>();

				glGenVertexArrays(1, &data.vao);
				glGenBuffers(1, &data.vbo);

				glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(T) * buffer.size(), &buffer[0], GL_STATIC_DRAW);

				glBindVertexArray(data.vao);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, vectSize, type, GL_FALSE, vectSize * sizeof(T), (void*)0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

				return data;
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

			Mat4 getModelMatrix(Vec3 position, Vec3 origin, Vec3 scale, float rotation);

			GLenum glCheckError_(const char* file, int line)
			{
				GLenum errorCode;
				while ((errorCode = glGetError()) != GL_NO_ERROR)
				{
					std::string error;
					switch (errorCode)
					{
					case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
					case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
					case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
					case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
					case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
					case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
					case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
					}
					std::cout << error << " | " << file << " (" << line << ")" << std::endl;
				}
				return errorCode;
			}

			

		}
	}
}

#define glCheckError() hagame::graphics::gl::glCheckError_(__FILE__, __LINE__) 

#endif