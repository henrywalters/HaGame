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

		class Color : public Vec4
		{
		public:

			Color() {}

			Color(float r, float g, float b) {
				vector[0] = r;
				vector[1] = g;
				vector[2] = b;
				vector[3] = 1.0f;
			}

			Color(float r, float g, float b, float a) {
				vector[0] = r;
				vector[1] = g;
				vector[2] = b;
				vector[3] = a;
			}

			Color(RGB rgb) {
				vector[0] = rgb.r;
				vector[1] = rgb.g;
				vector[2] = rgb.b;
				vector[3] = rgb.a;
			}

			Color(char* hex) {
				RGB rgb = RGB::ParseHex(hex);
				vector[0] = rgb.r;
				vector[1] = rgb.g;
				vector[2] = rgb.b;
				vector[3] = rgb.a;
			}

			~Color() {}

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

			static Color purple() {
				return Color(RGB({ 1.0, 0.0, 1.0, 1.0 }));
			}

			
		};
	}
}

#endif