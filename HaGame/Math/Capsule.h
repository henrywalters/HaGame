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
				Vec3 pos = center.a - Vec3(radius);
				Vec3 size = (center.b - center.a) + Vec3(radius * 2);
				return Cube(pos, size);
			}
		};
	}
}

#endif
