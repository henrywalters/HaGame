#ifndef PLANE_PRIMITIVE_H
#define PLANE_PRIMITIVE_H

#include "../Utils/Aliases.h"

namespace hagame {
	namespace math {
		struct Plane {
			// Normal vector of the plane
			Vec3 normal;

			// Distance of plane from the origin
			float distance;

			Plane(Vec3 _normal, float _distance) : normal(_normal), distance(_distance) {}

			Plane(Vec3 a, Vec3 b, Vec3 c) {
				normal = cross(b - a, c - a);
				distance = dot(normal, a);
			}

			// Computes the closest point on the plane of a point in 3D space
			Vec3 closestPoint(Vec3 point) {
				float t = (dot(normal, point) - distance) / dot2(normal);
				return point - normal * t;
			}

			// Computes the nearest distance to a point
			float distanceTo(Vec3 point) {
				return (dot(normal, point) - distance) / dot2(normal);
			}
		};
	}
}

#endif
