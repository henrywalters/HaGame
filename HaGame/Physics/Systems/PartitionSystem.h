#ifndef PARTITION_SYSTEM_H
#define PARTITION_SYSTEM_H

#include "./../IPartition.h"
#include "./../BoundingVolume.h"
#include "./../../Utils/SpatialMap.h"
#include "./../../Core/ECS/Entity.h"
#include "./../../Core/ECS/System.h"
#include "./../../Graphics/Routines.h"
#include "./../Components/Collider.h"

#include "./../GridPartition.h"

using namespace hagame::ecs;

namespace hagame {
	namespace physics {

		class PartitionSystem : public hagame::ecs::System {

			Ptr<IPartition<Entity>> m_staticEntities;
			Ptr<IPartition<Entity>> m_dynamicEntities;
			Vec3 m_chunkSize = Vec3(25.0f);

		public:

			PartitionSystem() {
				m_staticEntities = std::make_shared<GridPartition<Entity>>();
				m_dynamicEntities = std::make_shared<GridPartition<Entity>>();
			}

			IPartition<Entity>* getStaticEntities() { return m_staticEntities.get(); }
			IPartition<Entity>* getDynamicEntities() { return m_dynamicEntities.get(); }

			String getSystemName() { return "Partition System"; }

			void onSystemStart() {
				forEach<BoundingVolume>([this](BoundingVolume* volume, Ptr<Entity> entity) {
					if (volume->dynamic)
						m_dynamicEntities->insert(*volume, entity->transform->getPosition(), entity);
					else
						m_staticEntities->insert(*volume, entity->transform->getPosition(), entity);
				});
			}

			void onSystemStop() {
				m_staticEntities->clear();
				m_dynamicEntities->clear();
			}

			void onSystemUpdate(double dt) {
				m_dynamicEntities->clear();
				forEach<BoundingVolume>([this](BoundingVolume* volume, Ptr<Entity> entity) {
					if (volume->dynamic) {
						m_dynamicEntities->insert(*volume, entity->transform->getPosition(), entity);
					}
				});
			}



		};
	}
}

#endif
