#ifndef AABB_H
#define AABB_H

#include "../Utils/Aliases.h"
#include "OBB.h"

namespace hagame {
	namespace math {

		// Axis-aligned bounding-box represented with center point and half radii
		struct AABB {
			// The center of the bounding box
			Vec3 center;

			// The distance to each side of the box
			Vec3 radius;

			AABB() : center(Vec3::Zero()), radius(Vec3::Zero()) {}
			AABB(Vec3 _center, Vec3 _radius) : center(_center), radius(_radius) {}
			AABB(Cube cube) : center(cube.pos + (cube.size * 0.5)), radius(cube.size * 0.5) {}

			bool isIntersecting(AABB b) {
				if (abs(center[0] - b.center[0]) > (radius[0] + b.radius[0])) return false;
				if (abs(center[1] - b.center[1]) > (radius[1] + b.radius[1])) return false;
				if (abs(center[2] - b.center[2]) > (radius[2] + b.radius[2])) return false;
				return true;
			}

			// Given a point, compute the closest point on or in the bounding box to that point
			Vec3 closestPoint(Vec3 point) {
				Vec3 out;
				for (int i = 0; i < 3; i++) {
					float v = point[i];
					float m = center[i] - radius[i];
					float M = center[i] + radius[i];
					if (v < m) v = m;
					if (v > M) v = M;
					out[i] = v;
				}
				return out;
			}

			// Calculate the squared distance to the closest point on the AABB. Useful for collision calculations
			float sqDistClosestPoint(Vec3 point) {
				float sqDist = 0.0f;
				for (int i = 0; i < 3; i++) {
					float v = point[i];
					float m = center[i] - radius[i];
					float M = center[i] + radius[i];
					if (v < m)
						sqDist += (m - v) * (m - v);
					if (v > M)
						sqDist += (v - M) * (v - M);
				}
				return sqDist;
			}

			Cube getCube() {
				return Cube(center - radius, radius * 2.0);
			}

			Vec3 getMin() {
				return center - radius;
			}

			Vec3 getMax() {
				return center + radius;
			}

			String toString() {
				return "Center: " + center.toString() + " Radius: " + radius.toString();
			}

			OBB getOBB() {
				OBB obb;
				obb.center = center;
				obb.radius = radius;
				return obb;
			}
		};
	}
}

#endif
