#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <string>
#include "../../Vendor/entt/entt.hpp"

namespace hagame {
	namespace ecs {
		class Entity {
		private:

			friend class EntityManager;

			entt::basic_registry<uint32_t>* registry;

			Entity(uint32_t _entt_id, uint32_t _id) : id(_id), entt_id(_entt_id) {
				name = "Entity_" + _id;
			}

			const uint32_t entt_id;
			
		public:

			const uint32_t id;
			std::string name;

			template <class T>
			T* addComponent() {
				return &registry->emplace_or_replace<T>(entt_id);
			}

			template <class T>
			void removeComponent() {
				registry->remove_if_exists<T>(entt_id);
			}

			template <class T>
			T* getComponent() {
				return &registry->get<T>(entt_id);
			}
			
		};
	}
}

#endif