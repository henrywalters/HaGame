#ifndef RGB_H
#define RGB_H

#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		namespace color {
			struct RGB : public Vec4 {

				RGB() : RGB(0, 0, 0, 1) {}

				RGB(float r, float g, float b, float a = 1.0f) {
					vector[0] = r;
					vector[1] = g;
					vector[2] = b;
					vector[3] = a;
				}

				static RGB ParseHex(char* hex) {
					int r, g, b;
					sscanf_s(hex, "#%02x%02x%02x", &r, &g, &b);
					return RGB((float)(r / 255.0), (float)(g / 255.0), (float)(b / 255.0), 1.0);
				}
			};
		}
		
	}
}

#endif
