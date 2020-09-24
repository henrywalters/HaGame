namespace hagame {
	namespace graphics {
#pragma once

		struct RGB {
			float r;
			float g;
			float b;
			float a;
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

			Color(char* hex) {
				rgb = parseHex(hex);
			}

			~Color() {}

			static RGB red() {
				return RGB{ 1.0, 0.0, 0.0, 1.0 };
			}

			static RGB green() {
				return RGB{ 0.0, 1.0, 0.0, 1.0 };
			}

			static RGB blue() {
				return RGB{ 0.0, 0.0, 1.0, 1.0 };
			}

			static RGB white() {
				return RGB{ 1.0, 1.0, 1.0, 1.0 };
			}

			static RGB black() {
				return RGB{ 0.0, 0.0, 0.0, 1.0 };
			}

			static RGB parseHex(char* hex) {
				int r, g, b;
				sscanf_s(hex, "#%02x%02x%02x", &r, &g, &b);
				return RGB{ (float) (r / 255.0), (float)(g / 255.0), (float)(b / 255.0), 1.0 };
			}
		};
	}
}