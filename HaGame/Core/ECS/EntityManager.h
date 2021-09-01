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
			Map<uint32_t, Ptr<Entity>> enttMap;
			Map<uint32_t, Ptr<Entity>> idMap;
			int entityCount = 0;

		public:

			EntityManager(entt::basic_registry<uint32_t>* _registry) : registry(_registry) {
			}
			
			Ptr<Entity> add() {
				Ptr<Entity> entity = std::make_shared<Entity>(registry->create(), entityCount);
				entityCount++;
				entity->registry = registry;
				entity->transform = std::make_shared<hagame::Transform>();
				entity->transform->entity = entity.get();
				enttMap[entity->entt_id] = entity;
				idMap[entity->id] = entity;
				return entity;
			}

			Ptr<Entity> add(Ptr<Entity> parent) {
				Ptr<Entity> entity = add();
				entity->parent = parent;
				parent->children.push_back(entity);
				return entity;
			}

			Ptr<Entity> get(uint32_t id) {
				return idMap[id];
			}

			Ptr<Entity> getByEnttId(uint32_t id) {
				return enttMap[id];
			}

			void remove(Ptr<Entity> entity) {
				enttMap[entity->entt_id].reset();
				idMap[entity->id].reset();
				entity.reset();
			}

			void clear() {
				registry->clear();
			}

		};
	}
}

#endif