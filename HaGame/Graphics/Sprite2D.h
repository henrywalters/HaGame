#ifndef SPRITE_2D
#define SPRITE_2D

#include "ShaderProgram.h"
#include "Texture.h"
#include "Color.h"
#include "../Utils/Aliases.h"
#include "../Graphics/OpenGL.h"
#include "../Graphics/Primitives.h"
#include "../Graphics/VertexArray.h"
#include "../Graphics/VertexBuffer.h"
#include "./Quad.hpp"

namespace hagame {
	namespace graphics {

		class Sprite2D {
		public:
			Texture* texture;
			Ptr<Quad> quad;
			Vec3 pos;
			Color color = Color::white();

			void draw() {
				glActiveTexture(GL_TEXTURE0);
				texture->bind();
				quad->getMesh()->draw();
				glBindVertexArray(0);
			}

		};
	}
}

#endif
