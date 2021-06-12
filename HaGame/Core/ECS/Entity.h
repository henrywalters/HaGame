#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <string>
#include "../../Vendor/entt/entt.hpp"

namespace hagame {

	class Transform;

	namespace ecs {

		class EntityManager;
	
		class Entity {

		public:

			entt::basic_registry<uint32_t>* registry;

			Entity* parent;
			Array<Entity*> children;

			Ptr<Transform> transform;

			uint32_t entt_id;
			uint32_t id;
			std::string name;

			Entity(uint32_t _entt_id, uint32_t _id) : id(_id), entt_id(_entt_id) {
				name = "Entity_" + _id;
			}

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