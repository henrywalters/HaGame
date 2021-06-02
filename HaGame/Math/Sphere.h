#ifndef SPHERE_PRIMITIVE_H
#define SPHERE_PRIMITIVE_H

#include "../Utils/Aliases.h"

namespace hagame {
	namespace math {
		struct Sphere {
			Vec3 center;
			float radius;

			Sphere(Vec3 _center, float _radius) : center(_center), radius(_radius) {}

			bool isIntersecting(Sphere sphere) {
				Vec3 delta = sphere.center - center;
				float delta2 = dot2(delta);
				float radialSum = radius + sphere.radius;
				return delta2 <= radialSum * radialSum;
			}
		};
	}
}

#endif
