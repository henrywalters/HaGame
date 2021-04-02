#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include "EntityManager.h"
#include "../../Vendor/entt/entt.hpp"

namespace hagame {
	namespace ecs {
		class Entity {
		private:

			friend class EntityManager;

			uint32_t _id;
			entt::basic_registry<uint32_t>* _registry;

			Entity(uint32_t id, entt::basic_registry<uint32_t>* registry) : _id(id), _registry(registry) {}
			
		public:

			template <class T>
			T* addComponent() {
				return &_registry->emplace<T>(_id);
			}

			template <class T>
			void removeComponent() {
				registry->remove_if_exists<T>(_id);
			}

			template <class T>
			T* getComponent() {
				return &_registry->get<T>(_id);
			}
			
		};
	}
}

#endif