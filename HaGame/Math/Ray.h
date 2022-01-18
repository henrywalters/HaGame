#ifndef RAY_PRIMITIVE_H
#define RAY_PRIMITIVE_H

#include "../Utils/Aliases.h"
#include "./Line.h"
#include "./LineSegment.h"
#include "./Functions.h"

namespace hagame {
	namespace math {
		class Ray {

			const uint8_t RIGHT = 0;
			const uint8_t LEFT = 1;
			const uint8_t MIDDLE = 2;

		public:

			struct Hit {
				Vec3 position;
				Vec3 normal;
			};

			Vec3 origin;
			Vec3 direction;

			Ray(): origin(Vec3::Zero()), direction(Vec3::Zero()) {}

			Ray(Vec3 _origin, Vec3 _direction) : origin(_origin), direction(_direction) {}

			// Get a point on the parameterized line
			Vec3 getPointOnLine(float t) {
				if (t < 0.0f) t = 0.0f;
				return origin + direction * t;
			}

			// Calculates the point on the line closest to another point as well as the parameter t
			Vec3 closestPoint(Vec3 point, float& t) {
				t = dot(point - origin, direction);

				if (t <= 0.0f) {
					t = 0.0f;
					return origin;
				}
				else {
					float denom = dot2(direction);
					t = t / denom;
					return origin + direction * t;
				}
			}

			std::string toString() {
				return "Origin = " + origin.toString() + " Direction = " + direction.toString();
			}

			Line toLine() {
				return Line(origin, origin + direction);
			}

			LineSegment toLineSegment() {
				return LineSegment(origin, origin + direction);
			}
		};
	}
}

#endif
