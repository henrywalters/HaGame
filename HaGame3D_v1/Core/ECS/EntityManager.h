#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <cstdint>
#include "../../Vendor/entt/entt.hpp"
#include "Entity.h"

namespace hagame {
	namespace ecs {
		class EntityManager {
		private:

			entt::basic_registry<uint32_t>* _registry;

		public:

			EntityManager(entt::basic_registry<uint32_t>* registry) : _registry(registry) {}
			
			Entity add() {
				return Entity(_registry->create(), _registry);
			}

			void clear() {
				_registry->clear();
			}

		};
	}
}

#endif