#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#include "./Texture.h"
#include "./RawTexture.h"
#include <GL/glew.h>

namespace hagame {
	namespace graphics {
		class FrameBuffer {
		public:

			FrameBuffer() {}
			~FrameBuffer() {
				//glDeleteFrameBuffersEXT(1, &id);

				glDeleteFramebuffers(1, &id);
			}

			void initialize() {
				//glGenFrameBuffers(1, &id);
				glGenFramebuffers(1, &id);
			}

			void initializeRenderBufferObject(Vec2Int size) {
				glGenRenderbuffers(1, &rbo);

				glBindRenderbuffer(GL_RENDERBUFFER, rbo);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size[0], size[1]);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
			}

			void bind() {
				glBindFramebuffer(GL_FRAMEBUFFER, id);
			}

			void unbind() {
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			bool isComplete() {
				return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
			}

			void attachTexture(Texture* texture) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
			}

			template <GLuint TextureType>
			void attachRawTexture(RawTexture<TextureType>* texture) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
			}

			template <GLuint TextureType>
			void attachRawTexture(RawTexture<TextureType>* texture, int attachmentIdx) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIdx, GL_TEXTURE_2D, texture->id, 0);
			}

		private:
			unsigned int id, rbo;
		};
	}
}

#endif
