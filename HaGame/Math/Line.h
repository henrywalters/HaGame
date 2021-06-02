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
		};
	}
}

#endif