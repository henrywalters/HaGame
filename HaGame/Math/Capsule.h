#ifndef CAPSULE_H
#define CAPSULE_H

namespace hagame {
	namespace math {
		struct Capsule {
			LineSegment center;
			float radius;

			Capsule(LineSegment _center, float _radius) : center(_center), radius(_radius) {}
		};
	}
}

#endif
