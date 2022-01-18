#ifndef COLLIDER_H
#define COLLIDER_H

#include "../../Graphics/ShaderProgram.h"
#include "../../Utils/Aliases.h"
#include "../../Core/Transform.h"
#include "../../Math/AABB.h"
#include "./../boundingVolume.h"

namespace hagame {
	namespace physics {

		enum ColliderType {
			BoxCollider,
			//Plane,
			SphereCollider,
			//Mesh,
		};

		// Collider component ensures it is added to the spacial map.
		struct Collider {

			// A dynamic collider will update it's aabb every frame from the bounding cube and entity translation
			bool dynamic = true;

			// A non-active collider will not be considered in any collision queries
			bool active = true;

			// Control whether the collider types will be displayed
			bool display = false;

			// Control what type of collider to use
			ColliderType type = ColliderType::BoxCollider;

			// If you want to display the collider, you better freaking set this
			hagame::graphics::ShaderProgram* shader;

			// Required for BoxCollider
			std::optional<Cube> boundingCube = std::nullopt;

			// Required for SphereCollider
			std::optional<Sphere> boundingSphere = std::nullopt;

			// Ignore collisions with any of these specific entities
			Array<hagame::utils::UUID> ignoreEntities;

			// Ignore collisions with any entity that has any of these tags
			Array<String> ignoreTags;

		};
	}
}

#endif
