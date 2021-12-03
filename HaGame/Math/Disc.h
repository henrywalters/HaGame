#ifndef M_DISC_PRIMITIVE_H
#define M_DISC_PRIMITIVE_H

#include "../Utils/Aliases.h"
#include "Plane.h"

namespace hagame {
	namespace math {
		struct Disc {
			Vec3 center;
			Vec3 normal;
			float radius;

			Disc(Vec3 _center, Vec3 _normal, float _radius) : center(_center), normal(_normal), radius(_radius) {}
		};
	}
}

#endif
