#ifndef M_COLLISIONS_H
#define M_COLLISIONS_H

#include "Surface.h"
#include "Ray.h"
#include "NSphere.h"
#include "Hypercube.h"
#include "AABB.h"
#include "OBB.h"
#include "Capsule.h"
#include "Triangle.h"
#include "LineSegment.h"
#include "Plane.h"
#include "Disc.h"
#include "./../Utils/Aliases.h"

namespace hagame {
	namespace math {
		namespace collisions {
			struct Hit {
				Vec3 position;
				Vec3 normal;
				float depth;

				// Calculates the vector needed to offset the collision
				Vec3 calcOffset() {
					return position + normal * -depth;
				}
			};

			Optional<hagame::math::collisions::Hit> checkRayAgainstSphere(Ray ray, Sphere sphere, float &t);
			Optional<hagame::math::collisions::Hit> checkRayAgainstCube(Ray ray, Cube cube, float& t);
			Optional<hagame::math::collisions::Hit> checkRayAgainstCapsule(Ray ray, Capsule capsule, float& t);
			Optional<hagame::math::collisions::Hit> checkRayAgainstTriangle(Ray ray, Triangle tri, float& t);
			Optional<hagame::math::collisions::Hit> checkRayAgainstPlane(Ray ray, Plane plane, float& t);
			Optional<hagame::math::collisions::Hit> checkRayAgainstDisc(Ray ray, Disc disc, float& t);
			Optional<hagame::math::collisions::Hit> checkRayAgainstHollowDisc(Ray ray, Vec3 center, Vec3 normal, float innerRadius, float outerRadius, float& t);

			Optional<hagame::math::collisions::Hit> checkSphereAgainstLineSegment(Sphere sphere, LineSegment line);
			Optional<hagame::math::collisions::Hit> checkSphereAgainstSphere(Sphere A, Sphere B);
			Optional<hagame::math::collisions::Hit> checkSphereAgainstTriangle(Sphere sphere, Triangle tri);
			Optional<hagame::math::collisions::Hit> checkSphereAgainstSurface(Sphere sphere, Surface surface, Mat4 transform);

			Optional<hagame::math::collisions::Hit> checkCapsuleAgainstCapsule(Capsule A, Capsule B);
			Optional<hagame::math::collisions::Hit> checkCapsuleAgainstTriangle(Capsule capsule, Triangle tri);
			Optional<hagame::math::collisions::Hit> checkCapsuleAgainstSurface(Capsule A, Surface surface, Mat4 transform);
		}
	}
}

#endif
