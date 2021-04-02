#ifndef SPRITE
#define SPRITE
#include "ShaderProgram.h"
#include "Texture.h"
#include "Color.h"
#include "../Utils/Aliases.h"
#include "../Graphics/OpenGL.h"
#include "../Graphics/Primitives.h"

namespace hagame {
	namespace graphics {
		class Sprite {
		private:

			unsigned int VAO;

			void initializeForGL() {
				VAO = graphics::gl::loadVAO<float, 4>(graphics::QuadVertices);
			}

		public:
			Texture* texture;
			Rect rect;
			// The position of the origin relative to the sprites position <x, y>. Rotations will occur about the origin.
			Vec2 origin;
			float rotation;
			Color color;
			float zIndex;

			Sprite(Texture* _texture, Rect _rect = Rect::Zero(), float _rot = 0, Color _color = Color(1.0f, 1.0f, 1.0f, 1.0f), float _zIndex = 0.0f) : 
				texture(_texture), 
				rect(_rect),
				rotation(_rot), 
				color(_color),
				zIndex(_zIndex)
			{
				origin = rect.size * 0.5f;
				initializeForGL();
			}

			void draw(ShaderProgram* shader) {
				shader->use();
				glActiveTexture(GL_TEXTURE0);
				texture->bind();
				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);
			}

			Vec3 getPosition() {
				return Vec3({rect.pos[0], rect.pos[1], zIndex});
			}

			Vec3 getSize() {
				return Vec3({ rect.size[0], rect.size[1], 1.0f });
			}

			Vec3 getOrigin() {
				return Vec3({ origin[0], origin[1], 1.0f });
			}
		};
	}
}

#endif