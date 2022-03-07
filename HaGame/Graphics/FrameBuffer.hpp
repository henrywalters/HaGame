#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#include "./Texture.h"
#include <gl/glew.h>

namespace hagame {
	namespace graphics {
		class FrameBuffer {
		public:

			FrameBuffer() {}
			~FrameBuffer() {
				glDeleteFrameBuffers(1, &id);
			}

			void initialize() {
				glGenFrameBuffers(1, &id);
			}

			void bind() {
				glBindFrameBuffer(GL_FRAMEBUFFER, id);
			}

			void unbind() {
				glBindFrameBuffer(GL_FRAMEBUFFER, 0);
			}

			bool isComplete() {
				return glCheckFrameBufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
			}

			void attachTexture(Texture* texture) {
				glFrameBufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
			}

		private:
			unsigned int id;
		};
	}
}

#endif
