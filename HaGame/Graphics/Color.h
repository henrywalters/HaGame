#ifndef COLOR
#define COLOR
#include "../Utils/Aliases.h"
#include "rgb.h"
#include "hsv.h"

namespace hagame {
	namespace graphics {

		class Color : public color::RGB
		{
		public:

			Color() {}

			Color(int r, int g, int b) {
				vector[0] = (float) r / 255.0;
				vector[1] = (float) g / 255.0;
				vector[2] = (float) b / 255.0;
				vector[3] = 1.0f;
			}

			Color(int r, int g, int b, float a) {
				vector[0] = (float)r / 255.0;
				vector[1] = (float)g / 255.0;
				vector[2] = (float)b / 255.0;
				vector[3] = a;
			}

			Color(int r, int g, int b, int a) {
				vector[0] = (float) r / 255.0;
				vector[1] = (float) g / 255.0;
				vector[2] = (float) b / 255.0;
				vector[3] = (float)a / 255.0;
			}

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

			Color(color::RGB rgb) {
				vector[0] = rgb[0];
				vector[1] = rgb[1];
				vector[2] = rgb[2];
				vector[3] = rgb[3];
			}

			Color(char* hex) {
				color::RGB rgb = color::RGB::ParseHex(hex);
				vector[0] = rgb[0];
				vector[1] = rgb[1];
				vector[2] = rgb[2];
				vector[3] = rgb[3];
			}

			~Color() {}

			static Color red() {
				return Color(color::RGB{ 1.0, 0.0, 0.0, 1.0 });
			}

			static Color green() {
				return Color(color::RGB{ 0.0, 1.0, 0.0, 1.0 });
			}

			static Color blue() {
				return Color(color::RGB{ 0.0, 0.0, 1.0, 1.0 });
			}

			static Color white() {
				return Color(color::RGB{ 1.0, 1.0, 1.0, 1.0 });
			}

			static Color black() {
				return Color(color::RGB{ 0.0, 0.0, 0.0, 1.0 });
			}

			static Color purple() {
				return Color(color::RGB({ 1.0, 0.0, 1.0, 1.0 }));
			}

			static color::HSV RGB2HSV(color::RGB rgb) {
				color::HSV out;
				float min = rgb.xyz().min();
				float max = rgb.xyz().max();
				float delta = max - min;
				out[2] = max;

				if (delta < 0.00001 || max == 0.0) {
					out[0] = 0.0;
					out[1] = 0.0;
					return out;
				}

				if (max > 0) {
					out[1] = delta / max;
				}

				if (rgb[0] == max) {
					out[0] = (rgb[1] - rgb[2]) / delta;
				}
				else if (rgb[1] == max) {
					out[0] = 2.0 + (rgb[2] - rgb[0]) / delta;
				}
				else {
					out[0] = 4.0 + (rgb[0] - rgb[1]) / delta;
				}

				out[0] *= PI / 3;

				if (out[0] < 0.0) {
					out[0] += PI * 2;
				}

				return out;
			}

			static color::RGB HSV2RGB(color::HSV hsv) {
				float hh, p, q, t, ff;
				long i;
				color::RGB out;

				if (hsv[1] <= 0.0) {
					out[0] = hsv[2];
					out[1] = hsv[2];
					out[2] = hsv[2];
					return out;
				}

				hh = hsv[0];
				if (hh >= PI * 2)
					hh = 0.0;

				hh /= PI / 3;

				i = (long)hh;

				ff = hh - i;

				p = hsv[2] * (1.0 - hsv[1]);
				q = hsv[2] * (1.0 - hsv[1] * ff);
				t = hsv[2] * (1.0 - (hsv[1] * (1.0 - ff)));

				switch (i) {
				case 0:
					out[0] = hsv[2];
					out[1] = t;
					out[2] = p;
					break;
				case 1:
					out[0] = q;
					out[1] = hsv[2];
					out[2] = p;
					break;
				case 2:
					out[0] = p;
					out[1] = hsv[2];
					out[2] = t;
					break;

				case 3:
					out[0] = p;
					out[1] = q;
					out[2] = hsv[2];
					break;
				case 4:
					out[0] = t;
					out[1] = p;
					out[2] = hsv[2];
					break;
				case 5:
				default:
					out[0] = hsv[2];
					out[1] = p;
					out[2] = q;
					break;
				}

				return out;
			}

			static Color Lerp(Color start, Color end, float t) {
				auto startHsv = RGB2HSV(start);
				auto endHsv = RGB2HSV(end);
				auto startA = start[3];
				auto endA = end[3];

				auto lerpedHsv = startHsv + (endHsv - startHsv) * t;
				auto lerpedA = startA + (endA - startA) * t;

				auto rgb = HSV2RGB(color::HSV({ lerpedHsv[0], lerpedHsv[1], lerpedHsv[2] }));

				return Color({ rgb[0], rgb[1], rgb[2], lerpedA });
			}
		};
	}
}

#endif