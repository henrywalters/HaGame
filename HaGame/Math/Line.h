#ifndef LINE_PRIMITIVE_H
#define LINE_PRIMITIVE_H

#include "../Utils/Aliases.h"

namespace hagame {
	namespace math {
		struct Line {

			Vec3 a;
			Vec3 b;

			Line(Vec3 _a, Vec3 _b) : a(_a), b(_b) {}

			// Get a point on the parameterized line
			Vec3 getPointOnLine(float t) {
				return a + (b - a) * t;
			}

			// Calculates the point on the line closest to another point as well as the parameter t
			Vec3 closestPoint(Vec3 point, float& t) {
				Vec3 ab = b - a;
				t = dot(point - a, ab);

				float denom = dot2(ab);
				t = t / denom;
				return a + ab * t;
			}

			Vec3 center() {
				return getPointOnLine(0.5f);
			}

			Quat rotation() {
				return Quat(Vec3(b[2] - a[2], b[1] - a[1], b[0] - a[0]));
			}

			float magnitude() {
				return (b - a).magnitude();
			}

			float magnitudeSq() {
				return (b - a).magnitudeSq();
			}
		};

		void to_json(JSON& json, const Line& line) {
			json["a"] = line.a;
			json["b"] = line.b;
		}

		void from_json(const JSON& json, Line& line) {
			json.at("a").get_to(line.a);
			json.at("b").get_to(line.b);
		}
	}
}

#endif