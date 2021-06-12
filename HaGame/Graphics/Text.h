#ifndef TEXT_H
#define TEXT_H

#include "./../Utils/Aliases.h"
#include "./Font.h"
#include "./ShaderProgram.h"
#include "./Color.h"

namespace hagame {
	namespace graphics {
		class Text {
		private:
			unsigned int VAO, VBO;

		public:

			Text() {}

			~Text() {
				glDeleteVertexArrays(1, &VAO);
				glDeleteBuffers(1, &VBO);
			}

			void initializeForGL() {
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}

			void draw(Font* font, String message, Vec3 pos = Vec3::Zero(), float maxLength = 0.0f) {

				glActiveTexture(GL_TEXTURE0);
				glBindVertexArray(VAO);

				String::const_iterator c;

				float x = pos[0];
				float y = pos[1];

				for (c = message.begin(); c != message.end(); c++) {
					FontCharacter character = font->getChar(*c);

					if (*c == '\n') {
						x = pos[0];
						y += font->getLineGap() == 0 ? character.ascent[0] - character.ascent[1] * 2 : font->getLineGap();
						continue;
					}

					float xPos = x + character.bearing[0] * font->getScale();
					float yPos = y - character.ascent[1];
					float yPos2 = y - character.ascent[0];

					Vec2 size = character.size.cast<float>() * 0.5;

					//std::cout << font->getScale() << std::endl;
					//std::cout << size.toString() << std::endl;

					float vertices[6][4] = {
						{xPos, yPos2, 0.0f, 0.0f},
						{xPos, yPos, 0.0f, 1.0f},
						{xPos + size[0], yPos, 1.0f, 1.0f},
						{xPos, yPos2, 0.0f, 0.0f},
						{xPos + size[0], yPos, 1.0f, 1.0f},
						{xPos + size[0], yPos2, 1.0f, 0.0f}
					};

					glBindTexture(GL_TEXTURE_2D, character.id);

					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glDrawArrays(GL_TRIANGLES, 0, 6);

					// glCheckError();

					x += character.advance * font->getScale() * 0.75;
				}
			}
		};
	}
}

#endif
