#ifndef RAY_PRIMITIVE_H
#define RAY_PRIMITIVE_H

#include "../Utils/Aliases.h"
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

			std::optional<Hit> checkSphere(Sphere sphere, float &t) {
				std::optional<Hit> hit;

				Vec3 f = origin - sphere.center;
				float a = dot(direction, direction);
				float b = 2 * dot(f, direction);
				float c = dot(f, f) - sphere.radius * sphere.radius;

				float disc = b * b - 4 * a * c;

				if (disc < 0) {
					return std::nullopt;
				}
				else {
					disc = sqrt(disc);
					float t1 = (-b - disc) / (2 * a);
					float t2 = (-b + disc) / (2 * a);

					if (t1 >= 0 && t1 <= 1) {
						t = t1;
					}
					else if (t2 >= 0 && t2 <= 1) {
						t = t2;
					}
					else {
						return std::nullopt;
					}

					Vec3 pos = getPointOnLine(t);
					Vec3 norm = (pos - sphere.center).normalized();

					hit = Hit{ pos, norm };

					return hit;
				}
			}

			// Based on the code from the absoltu Graphics Gem Excerpt: https://github.com/erich666/GraphicsGems/blob/master/gems/RayBox.c
			std::optional<Hit> checkCube(Cube cube, float &t) {

				std::optional<Hit> hit = std::nullopt;

				bool inside = true;
				int quadrant[3];
				int whichPlane;
				float maxT[3];
				Vec3 cubeMin = cube.min();
				Vec3 cubeMax = cube.max();
				float candidatePlane[3];

				int i;

				for (i = 0; i < 3; i++) {
					if (origin[i] < cubeMin[i]) {
						quadrant[i] = LEFT;
						candidatePlane[i] = cubeMin[i];
						inside = false;
					}
					else if (origin[i] > cubeMax[i]) {
						quadrant[i] = RIGHT;
						candidatePlane[i] = cubeMax[i];
						inside = false;
					}
					else {
						quadrant[i] = MIDDLE;
					}
				}

				if (inside) {
					t = 0;
					hit = Hit{Vec3::Zero(), Vec3::Zero()};
					return hit;
				}

				for (i = 0; i < 3; i++)
					maxT[i] = quadrant[i] != MIDDLE && direction[i] != 0.0f ? (candidatePlane[i] - origin[i]) / direction[i] : -1.0f;

				whichPlane = 0;
				for (i = 1; i < 3; i++) {
					if (maxT[whichPlane] < maxT[i])
						whichPlane = i;
				}

				if (maxT[whichPlane] < 0.0f || maxT[whichPlane] > 1.0f) return std::nullopt;

				for (i = 0; i < 3; i++) {
					if (whichPlane != i) {
						float pos = origin[i] + maxT[whichPlane] * direction[i];
						if (pos < cubeMin[i] || pos > cubeMax[i])
							return std::nullopt;
					}
				}

				Vec3 norm;
				norm[whichPlane] = quadrant[whichPlane] == 1 ? -1.0f : 1.0f;

				t = maxT[whichPlane];
				hit = Hit{getPointOnLine(t), norm};
				return hit;
			}

			std::string toString() {
				return "Origin = " + origin.toString() + " Direction = " + direction.toString();
			}
		};
	}
}

#endif
