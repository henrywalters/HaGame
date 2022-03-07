#ifndef TEXT_H
#define TEXT_H

#include "./../Utils/Aliases.h"
#include "./Font.h"
#include "./ShaderProgram.h"
#include "./Color.h"

namespace hagame {
	namespace graphics {

		enum class TextHAlignment {
			Left,
			Center,
			Right
		};

		enum class TextVAlignment {
			Top,
			Center,
			Bottom
		};

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

			void draw(Font* font, String message, Vec3 pos = Vec3::Zero(), TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Top) {

				glActiveTexture(GL_TEXTURE0);
				glBindVertexArray(VAO);

				String::const_iterator c;

				Vec2 msgSize = font->calculateMessageSize(message);
				auto messageParts = stringSplit(message, '\n');

				float x = pos[0];
				float y = pos[1] - (font->getDescent() * font->getScale()); // +(msgSize[1] * 0.5f);

				if (messageParts.size() > 1) {
					y += font->calculateLineSpacing();
				}

				/*switch (alignmentV) {
				case TextVAlignment::Bottom:
					break;
				case TextVAlignment::Center:
					y += (font->getAscent() + font->getDescent()) * font->getScale() * 0.5f;
					break;
				case TextVAlignment::Top:
					y += font->getAscent() * font->getScale();
					break;
				}*/

				for (auto part : messageParts) {
					
					Vec2 partSize = font->calculateMessageSize(part);

					for (c = part.begin(); c != part.end(); c++) {
						auto character = font->getChar(*c);

						if (*c == '\t') {
							x += 4 * font->getChar(' ')->advance * font->getScale();
							continue;
						}

						Vec2 size = character->size.cast<float>();

						float xPos = x + character->bearing[0] * font->getScale();

						switch (alignmentH) {
						case TextHAlignment::Left:
							break;
						case TextHAlignment::Center:
							xPos += (msgSize[0] - partSize[0]) * 0.5f;
							break;
						case TextHAlignment::Right:
							xPos += msgSize[0] - partSize[0];
							break;
						}

						float yPos = y - character->ascent[1];
						//float yPos2 = y - character.ascent[0];
						float yPos2 = yPos + size[1];

						float vertices[6][4] = {
							{xPos, yPos2, 0.0f, 0.0f},
							{xPos, yPos, 0.0f, 1.0f},
							{xPos + size[0], yPos, 1.0f, 1.0f},
							{xPos, yPos2, 0.0f, 0.0f},
							{xPos + size[0], yPos, 1.0f, 1.0f},
							{xPos + size[0], yPos2, 1.0f, 0.0f}
						};


						glBindTexture(GL_TEXTURE_2D, character->id);

						glBindBuffer(GL_ARRAY_BUFFER, VBO);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glDrawArrays(GL_TRIANGLES, 0, 6);

						// glCheckError();

						x += character->advance * font->getScale();
					}

					x = pos[0];
					y -= font->calculateLineSpacing();
				}

				
			}
		};
	}
}

#endif
