#ifndef CAPSULE_H
#define CAPSULE_H

#include "./../Utils/Aliases.h"

namespace hagame {
	namespace math {
		struct Capsule {
			LineSegment center;
			float radius;

			Capsule(LineSegment _center, float _radius) : center(_center), radius(_radius) {}

			Cube getBoundingCube() {
				return Cube(Vec3(center.a - Vec3(radius)), Vec3(radius * 2, (center.b - center.a).magnitude() + radius * 2, radius * 2));
			}
		};
	}
}

#endif
