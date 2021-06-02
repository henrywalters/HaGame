#ifndef COLLIDER_H
#define COLLIDER_H

#include "../../Graphics/ShaderProgram.h"
#include "../../Utils/Aliases.h"
#include "../../Core/Transform.h"
#include "../../Math/AABB.h"

namespace hagame {
	namespace physics {

		enum ColliderType {
			Box,
			Plane,
			Sphere,
			Mesh,
		};

		// Collider component ensures it is added to the spacial map.
		struct Collider {

			// A dynamic collider will update it's aabb every frame from the bounding cube and entity translation
			bool dynamic = true;

			// A non-active collider will not be considered in any collision queries
			bool active = true;

			// Control whether the collider types will be displayed
			bool display = false;

			ColliderType type = ColliderType::Box;

			hagame::graphics::ShaderProgram* shader;

			Cube boundingCube;

			hagame::math::AABB aabb;

			void updateAABB(Mat3 model, Vec3 translation) {
				aabb = hagame::math::AABB(transformBoundingBox(boundingCube, model, translation));
			}
		};
	}
}

#endif
