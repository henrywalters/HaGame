#ifndef FONT_H
#define FONT_H

#include <gl/glew.h>
#include "./../Utils/Aliases.h"
#include "./../Utils/File.h"
#include "./../Vendor/imgui/imstb_truetype.h"

namespace hagame {
	namespace graphics {

		struct FontCharacter {
			unsigned int id;
			Vec2Int size;
			Vec2Int offset;
			Vec2Int bearing;
			Vec2Int ascent;
			int advance;

			String toString() {
				return "ID: " + toStr(id) + " Size: " + size.toString() + " Offset: " + offset.toString() + " Bearing: " + bearing.toString() + "Ascent: " + ascent.toString() + " Advance: " + toStr(advance);
			}

			~FontCharacter() {
				glDeleteTextures(1, &id);
			}
		};

		typedef Map<char, Ptr<FontCharacter>> FontMap;

		class Font {

		private:
			float scale;
			stbtt_fontinfo font;
			int fontSize;
			int ascent, descent, lineGap;
			FontMap fontMap;
			float lineSpacing = 1.0;

			Ptr<FontCharacter> loadCharacter(char ch) {
				Ptr<FontCharacter> character = std::make_shared<FontCharacter>();
				int x0, x1, y0, y1;
				unsigned char* buffer = stbtt_GetCodepointBitmap(&font, 0, scale, ch, &character->size[0], &character->size[1], &character->offset[0], &character->offset[1]);
				//unsigned char* buffer = stbtt_GetCodepointBitmapSubpixel(&font, 0, scale, 0, 0, ch, &character->size[0], &character->size[1], &character->offset[0], &character->offset[1]);
				stbtt_GetCodepointHMetrics(&font, ch, &character->advance, &character->bearing[0]);
				stbtt_GetCodepointBitmapBox(&font, ch, 0, scale, 0, &character->ascent[0], 0, &character->ascent[1]);
				//stbtt_GetCodepointBitmapBoxSubpixel(&font, ch, 0, scale, 0, 0, 0, &character->ascent[0], 0, &character->ascent[1]);

				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glGenTextures(1, &character->id);
				glBindTexture(GL_TEXTURE_2D, character->id);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					character->size[0],
					character->size[1],
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					buffer
				);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				delete buffer;

				return character;
			}

			// Create a map of font character textures
			void loadCharacterMap(int startIdx = 0, int endIdx = 128) {
				fontMap.clear();
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				for (int i = startIdx; i <= endIdx; i++) {
					fontMap.insert(std::make_pair<char, Ptr<FontCharacter>>((char) i, loadCharacter((char) i)));
				}
			}

		public:
			Font(hagame::utils::File file, int _fontSize = 24) {
				if (!stbtt_InitFont(&font, file.getBuffer().data, 0)) {
					std::cout << "Failed to initialize font: " << file.name << std::endl;
					throw new std::exception("Failed to initialize True type font. Check console for more info");
				}
				
				setFontSize(_fontSize);

			}

			void setFontSize(int _fontSize) {
				fontSize = _fontSize;
				scale = stbtt_ScaleForPixelHeight(&font, fontSize);
				stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
				loadCharacterMap();
			}

			int getFontSize() {
				return fontSize;
			}

			float getScale() {
				return scale;
			}

			int getLineSpacing() {
				return lineSpacing;
			}

			void setLineSpacing(int _lineSpacing) {
				lineSpacing = _lineSpacing;
			}

			int getAscent() { return ascent; }
			int getDescent() { return descent; }
			int getLineGap() { return lineGap; }

			FontCharacter* getChar(char character) {
				if (fontMap.find(character) == fontMap.end()) {
					throw new std::exception("Character not loaded into font map");
				}
				return fontMap[character].get();
			}

			float calculateLineSpacing() {
				return (ascent - descent + lineGap) * lineSpacing * scale;
			}

			float calculateTabSpacing() {
				return 4 * getChar(' ')->advance * scale;
			}

			Vec2 calculateMessageSize(String msg) {
				Vec2 size;
				float x = 0;
				float y = calculateLineSpacing();

				String::const_iterator c;

				for (c = msg.begin(); c != msg.end(); c++) {
					if (*c == '\n') {
						size[0] = x;
						x = 0;
						y += calculateLineSpacing();
						continue;
					}

					if (*c == '\t') {
						x += calculateTabSpacing();
						continue;
					}

					auto ch = getChar(*c);
					auto size = ch->size.cast<float>();

					x += ch->advance * scale;
				}

				if (x != 0)
					size[0] = x;

				size[1] = y;

				return size;
			}
			
		};
	}
}

#endif
