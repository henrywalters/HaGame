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
			std::vector<Entity> entities;

		public:

			EntityManager(entt::basic_registry<uint32_t>* _registry) : registry(_registry) {
				entities = std::vector<Entity>();
			}
			
			Entity* add() {
				Entity entity = Entity(registry->create(), entities.size());
				entity.registry = registry;
				entities.push_back(entity);
				return &entity;
			}

			Entity* get(uint32_t id) {
				return &entities[id];
			}

			void clear() {
				registry->clear();
			}

		};
	}
}

#endif