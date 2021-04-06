#ifndef ECS_H
#define ECS_H

#include <cstdint>
#include "SystemManager.h"
#include "EntityManager.h"
#include "../../Vendor/entt/entt.hpp"

namespace hagame {
	namespace ecs {

		/*
		* @brief The ECS class provides a single set of entities and systems on top of the entt registry. See the wiki for more details: https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#types-const-non-const-and-all-in-between for more details.
		*/ 
		class ECS {
		private:
			entt::basic_registry<uint32_t> registry;

		public:
			EntityManager entities;
			SystemManager systems;

			ECS(): entities(EntityManager(&registry)), systems(SystemManager(&registry)) {}
			~ECS() {}

			entt::basic_registry<uint32_t>* getRegistry() {
				return &registry;
			}
		};
	}
}

#endif
