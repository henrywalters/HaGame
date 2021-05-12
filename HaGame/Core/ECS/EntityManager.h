#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <cstdint>
#include "../../Vendor/entt/entt.hpp"
#include "Entity.h"

namespace hagame {
	namespace ecs {
		class EntityManager {
		private:

			entt::basic_registry<uint32_t>* registry;
			std::vector<Ptr<Entity>> entities;
			Map<uint32_t, Ptr<Entity>> enttMap;
			Map<uint32_t, Ptr<Entity>> idMap;

		public:

			EntityManager(entt::basic_registry<uint32_t>* _registry) : registry(_registry) {
				entities = std::vector<Ptr<Entity>>();
			}
			
			Entity* add() {
				Ptr<Entity> entity = std::make_shared<Entity>(registry->create(), entities.size());
				entity->registry = registry;
				entities.push_back(entity);
				enttMap[entity->entt_id] = entity;
				idMap[entity->id] = entity;
				return entity.get();
			}

			Entity* get(uint32_t id) {
				return idMap[id].get();
			}

			Entity* getByEnttId(uint32_t id) {
				return enttMap[id].get();
			}

			void clear() {
				registry->clear();
			}

		};
	}
}

#endif