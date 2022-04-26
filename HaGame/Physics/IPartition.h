#ifndef IPARTITION_H
#define IPARTITION_H

#include "./../Utils/Aliases.h"
#include "./boundingVolume.h"
#include "./../Math/AABB.h"

namespace hagame {
	namespace physics {

		template <class T>
		using VisitNeighborFn = std::function<void(Ptr<T>)>;

		template <class T>
		struct IPartition {
			virtual void insert(BoundingVolume volume, Vec3 pos, RawPtr<T> object) {
				throw new std::exception("Unimplemented");
			};

			virtual Array<RawPtr<T>> getNeighbors(Vec3 point) {
				throw new std::exception("Unimplemented");
			}

			virtual Array<RawPtr<T>> getNeighbors(BoundingVolume* volume, Vec3 pos) {
				throw new std::exception("Unimplemented");
			}

			virtual void clear() {
				throw new std::exception("Unimplemented");
			};

			virtual math::AABB getPartition(Vec3 point) {
				throw new std::exception("Unimplemented");
			};
		};
	}
}

#endif
