#ifndef BOUNDING_VOLUME_H
#define BOUNDING_VOLUME_H

#include "./../Utils/Aliases.h"
#include "./../Math/Collisions.h"
#include "./../Math/Capsule.h"
#include "./../Math/Surface.h"
#include "./../Math/AABB.h"

namespace hagame {
	namespace physics {

		enum class BoundingVolumeType {
			Sphere,
			Cube,
			Capsule,
			Surface,
		};

		class BoundingVolume {
			BoundingVolumeType m_type;
			Optional<Sphere> m_sphere = std::nullopt;
			Optional<Cube> m_cube = std::nullopt;
			Optional<math::Capsule> m_capsule = std::nullopt;
			Optional<math::Surface> m_surface = std::nullopt;

		public:

			bool dynamic = false;

			BoundingVolume(Cube cube) : m_type(BoundingVolumeType::Cube), m_cube(cube) {}
			BoundingVolume(Sphere sphere) : m_type(BoundingVolumeType::Sphere), m_sphere(sphere) {}
			BoundingVolume(math::Capsule capsule) : m_type(BoundingVolumeType::Capsule), m_capsule(capsule) {}
			BoundingVolume(math::Surface surface) : m_type(BoundingVolumeType::Surface), m_surface(surface) {}

			math::AABB getBoundingBox() {
				switch (m_type) {
				case BoundingVolumeType::Cube:
					return math::AABB(getCube());
				case BoundingVolumeType::Capsule:
					return math::AABB(getCapsule().getBoundingCube());
				case BoundingVolumeType::Sphere:
					return math::AABB(getSphere().getBoundingCube());
				case BoundingVolumeType::Surface:
					return math::AABB(getSurface().getBoundingCube());
				default:
					throw new std::exception("Unhandled bounding volume type");
				}
			}

			BoundingVolumeType getType() { return m_type; }

			Sphere getSphere() {
				assert(m_type == BoundingVolumeType::Sphere && m_sphere.has_value());
				return m_sphere.value();
			}

			math::Capsule getCapsule() {
				assert(m_type == BoundingVolumeType::Capsule && m_capsule.has_value());
				return m_capsule.value();
			}

			Cube getCube() {
				assert(m_type == BoundingVolumeType::Cube && m_cube.has_value());
				return m_cube.value();
			}

			math::Surface getSurface() {
				assert(m_type == BoundingVolumeType::Surface && m_surface.has_value());
				return m_surface.value();
			}

		};
	}
}

#endif