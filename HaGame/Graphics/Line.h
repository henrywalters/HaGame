#ifndef LINE_H
#define LINE_H

#include "../Utils/Aliases.h"
#include "Color.h"
#include "ShaderProgram.h"
#include "../Math/Line.h"

namespace hagame {
	namespace graphics {
		class Line {

		public:

			Color color;

			Line(Vec3 _p1, Vec3 _p2, Color _color, float _width = 1.0f) : p1(_p1), p2(_p2), color(_color), width(_width) {
				updateVertices();
				initializeForGL();
			}

			Line(math::Line line, Color _color, float _width = 1.0f): p1(line.a), p2(line.b), color(_color), width(_width) {
				updateVertices();
				initializeForGL();
			}

			~Line() {
				removeBuffers();
			}

			void setP1(Vec3 _p1) {
				p1 = _p1;
				updateVertices();
			}

			void setP2(Vec3 _p2) {
				p2 = _p2;
				updateVertices();
			}

			void draw(ShaderProgram* shader) {
				shader->use();
				glBindVertexArray(VAO);
				glDrawArrays(GL_LINES, 0, 2);
				glBindVertexArray(0);
			}

		private:
			unsigned int VAO, VBO;

			Ptr<hagame::graphics::VertexBuffer<Vec3>> vbo;
			Ptr<hagame::graphics::VertexArray> vao;

			Vec3 p1;
			Vec3 p2;
			float width;

			Array<float> vertices;

			void updateVertices() {

				auto delta = p2 - p1;
				auto zero = Vec3::Zero();
				Vec3 perpY, perpX;
				
				if (delta.dot(Vec3::Top()) == 0.0f)
					perpX = delta.cross(Vec3::Right());
				else
					perpX = delta.cross(Vec3::Top());

				perpX.normalize();

				perpY = delta.cross(perpX);

				perpY.normalize();

				perpX *= width * 0.5f;
				perpY *= width * 0.5f;

				vertices = {
					p1[0], p1[1], p1[2],
					p2[0], p2[1], p2[2]
				};
			}

			void initializeForGL() {

				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);

				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);

				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

				glDisableVertexAttribArray(0);
				
			}

			void removeBuffers() {
				glDeleteBuffers(1, &VBO);
				glDeleteVertexArrays(1, &VAO);
			}
		};
	}
}

#endif
