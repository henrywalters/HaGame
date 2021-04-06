#ifndef LINE_H
#define LINE_H

#include "../Utils/Aliases.h"
#include "Color.h"
#include "ShaderProgram.h"

namespace hagame {
	namespace graphics {
		class Line {
		private:
			unsigned int VAO, VBO;

			void initializeForGL() {
				std::vector<float> vertices = {
					p1[0], p1[1], p1[2],
					p2[0], p2[1], p2[2]
				};

				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glBindVertexArray(VAO);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}

		public:

			Vec3 p1;
			Vec3 p2;
			Color color;

			Line(Vec3 _p1, Vec3 _p2, Color _color) : p1(_p1), p2(_p2), color(_color) {
				initializeForGL();
			}

			void draw(ShaderProgram* shader) {
				shader->use();
				glBindVertexArray(VAO);
				glDrawArrays(GL_LINES, 0, 2);
				glBindVertexArray(0);
			}
		};
	}
}

#endif
