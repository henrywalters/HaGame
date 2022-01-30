#ifndef SPRITE
#define SPRITE
#include "ShaderProgram.h"
#include "Texture.h"
#include "Color.h"
#include "../Utils/Aliases.h"
#include "../Graphics/OpenGL.h"
#include "../Graphics/Primitives.h"
#include "../Graphics/VertexArray.h"
#include "../Graphics/VertexBuffer.h"

namespace hagame {
	namespace graphics {

		// Deprecated in favor of Sprite2D
		class Sprite {
		private:

			Ptr<VertexBuffer<Vec4>> vbo;
			Ptr<VertexArray> vao;
			GLuint VAO;

			gl::VertexBufferArray VBA;

			void initializeForGL() {
				// VAO = graphics::gl::loadVAO<float, 4>(graphics::QuadVertices);
				//VBA = graphics::gl::loadVBA<float, 4>(graphics::QuadVertices);
				vbo = VertexBuffer<Vec4>::Static(graphics::QuadVertexVectors);
				vao = std::make_shared<VertexArray>();
				vao->initialize();
				vao->defineAttribute<Vec4>(vbo.get(), DataType::Float, 0, 4);

			}

		public:
			Texture* texture;
			Rect rect;
			// The position of the origin relative to the sprites position <x, y>. Rotations will occur about the origin.
			Vec2 origin;
			float rotation;
			Color color;
			float zIndex;

			Sprite(): texture(NULL), rect(Rect::Zero()), rotation(0.0f), color(Color::white()), zIndex(0) {
				initializeForGL();
			}

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

			~Sprite() {
				vbo->clear();
				//glBindBuffer(GL_ARRAY_BUFFER, VBA.vbo);
				//glClearBufferData(GL_ARRAY_BUFFER, GL_RG32F, GL_RG32F, GL_FLOAT, NULL);
			}

			void draw(ShaderProgram* shader) {
				shader->use();
				shader->setVec4("color", color);
				glActiveTexture(GL_TEXTURE0);
				texture->bind();
				vao->bind();
				// glBindVertexArray(VBA.vao);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glCheckError();
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