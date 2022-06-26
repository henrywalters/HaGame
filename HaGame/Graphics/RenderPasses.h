#ifndef RENDER_PASSES_H
#define RENDER_PASSES_H

#include "./FrameBuffer.hpp"

namespace hagame {
	namespace graphics {

		struct RenderPass {
			Ptr<RawTexture<GL_RGBA>> texture;
			Ptr<FrameBuffer> buffer;
		};

		// RenderPasses provides the ability to easily switch render modes
		template <class KeyType>
		class RenderPasses {
		public:

			void removeAll() {
				m_passes.clear();
			}

			RawPtr<RenderPass> create(KeyType key, Vec2Int size) {
				auto pass = std::make_shared<RenderPass>();
				pass->buffer = std::make_shared<FrameBuffer>();
				pass->buffer->initialize();
				pass->buffer->bind();
				pass->buffer->initializeRenderBufferObject(size);
				pass->texture = std::make_shared<RawTexture<GL_RGBA>>(size);
				pass->buffer->attachRawTexture(pass->texture.get());
				m_passes.insert(std::make_pair(key, pass));
				return pass.get();
			}

			RawPtr<RenderPass> get(KeyType key) {
				if (m_passes.find(key) == m_passes.end()) {
					throw new std::exception("RenderPass does not exist");
				}

				return m_passes[key].get();
			}

			void bind(KeyType key) {
				m_passes[key]->buffer->bind();
			}

			void unbind(KeyType key) {
				m_passes[key]->buffer->unbind();
			}

			void clear(KeyType key, Color color) {
				m_passes[key]->buffer->bind();
				glClearColor(color[0], color[1], color[2], color[3]);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}

			template <size_t attachments = 1>
			void render(KeyType key) {
				bind(key);
				unsigned int attach[attachments];
				for (int i = 0; i < attachments; i++) {
					attach[i] = GL_COLOR_ATTACHMENT0 + i;
				}
				glDrawBuffers(1, attach);
				unbind(key);
			}

			void activateTextures() {
				int idx = 0;
				for (auto& [key, pass] : m_passes) {
					glActiveTexture(GL_TEXTURE0 + idx++);
					pass->texture->bind();
				}
			}

		private:

			Map<KeyType, Ptr<RenderPass>> m_passes;
		};
	}
}

#endif
