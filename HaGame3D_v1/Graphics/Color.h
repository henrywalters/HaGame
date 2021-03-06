#ifndef COLOR
#define COLOR
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		struct RGB {
			float r;
			float g;
			float b;
			float a;

			static RGB ParseHex(char* hex) {
				int r, g, b;
				sscanf_s(hex, "#%02x%02x%02x", &r, &g, &b);
				return RGB{ (float)(r / 255.0), (float)(g / 255.0), (float)(b / 255.0), 1.0 };
			}
		};

		class Color
		{
		public:

			RGB rgb;

			Color() {}

			Color(float r, float g, float b) {
				Color(r, g, b, 1.0);
			}

			Color(float r, float g, float b, float a) {
				rgb.r = r;
				rgb.g = g;
				rgb.b = b;
				rgb.a = a;
			}

			Color(RGB _rgb) {
				rgb = _rgb;
			}

			Color(char* hex) {
				rgb = RGB::ParseHex(hex);
			}

			~Color() {}

			glm::vec4 getVec4() {
				return glm::vec4(rgb.r, rgb.g, rgb.b, rgb.a);
			}

			static Color red() {
				return Color(RGB{ 1.0, 0.0, 0.0, 1.0 });
			}

			static Color green() {
				return Color(RGB{ 0.0, 1.0, 0.0, 1.0 });
			}

			static Color blue() {
				return Color(RGB{ 0.0, 0.0, 1.0, 1.0 });
			}

			static Color white() {
				return Color(RGB{ 1.0, 1.0, 1.0, 1.0 });
			}

			static Color black() {
				return Color(RGB{ 0.0, 0.0, 0.0, 1.0 });
			}

			
		};
	}
}

#endif