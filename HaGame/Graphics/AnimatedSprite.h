#ifndef ANIMATED_SPRITE
#define ANIMATED_SPRITE
#include "ShaderProgram.h"
#include "Texture.h"
#include "Color.h"
#include "../Utils/Aliases.h"
#include "../Graphics/OpenGL.h"
#include "../Graphics/Primitives.h"

namespace hagame {
	namespace graphics {
		class AnimatedSprite {
		private:

			unsigned int VAO;

			void initializeForGL() {
				VAO = graphics::gl::loadVAO<float, 4>(graphics::QuadVertices);
			}

			int currentIndex = 0;
			double currentDt = 0.0f;

		public:
			Array<Texture*> textures;
			Rect rect;
			// The position of the origin relative to the sprites position <x, y>. Rotations will occur about the origin.
			Vec2 origin;
			float rotation;
			Color color;
			float zIndex;
			int fps = 30;

			AnimatedSprite() : textures({}), rect(Rect::Zero()), rotation(0.0f), color(Color::white()), zIndex(0) {
				initializeForGL();
			}

			AnimatedSprite(Array<Texture*> _textures, Rect _rect = Rect::Zero(), int _fps = 24, float _rot = 0, Color _color = Color(1.0f, 1.0f, 1.0f, 1.0f), float _zIndex = 0.0f) : 
				textures(_textures), 
				rect(_rect),
				fps(_fps),
				rotation(_rot), 
				color(_color),
				zIndex(_zIndex)
			{
				origin = rect.size * 0.5f;
				initializeForGL();
			}

			void update(double dt) {
				currentDt += dt;
				if (currentDt >= (1.0f / (float) fps)) {
					currentIndex = (currentIndex + 1) % textures.size();
					currentDt = 0;
				}
			}

			void draw(ShaderProgram* shader) {
				shader->use();
				shader->setVec4("color", color);
				glActiveTexture(GL_TEXTURE0);
				textures[currentIndex]->bind();
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