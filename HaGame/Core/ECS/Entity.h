#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <string>
#include "./Component.h"
#include "../../Vendor/entt/entt.hpp"
#include "../../Utils/UUID.h"

namespace hagame {

	class Transform;

	namespace ecs {

		class EntityManager;
	
		class Entity {

		public:

			Set<String> tags;

			entt::basic_registry<uint32_t>* registry;

			Ptr<Entity> parent;
			Array<Ptr<Entity>> children;
			// Array<Component*> components;

			Ptr<Transform> transform;

			uint32_t entt_id;
			utils::UUID id;
			utils::UUID uuid;
			std::string name;

			Entity(uint32_t _entt_id, uint32_t entityCount) : id(hagame::utils::UUID()), entt_id(_entt_id) {
				name = "Entity_" + std::to_string(entityCount);
			}

			~Entity() {
				registry->destroy(entt_id);
			}

			void addTag(String tag) {
				tags.insert(tag);
			}

			// Check if entity has the given tag
			bool hasTag(String tag) {
				return hasKey(tags, tag);
			}

			// Check if entity has at least one of the tags
			bool hasTag(Array<String> tags) {
				for (String tag : tags) {
					if (hasTag(tag)) return true;
				}
				return false;
			}

			template <class T>
			T* addComponent() {
				T* component = &registry->emplace<T>(entt_id);
				return component;
			}

			void addComponent(entt::id_type componentId) {
				// &registry->emplace()
			}

			template <class T>
			void removeComponent() {
				registry->remove_if_exists<T>(entt_id);
			}

			template <class T>
			bool hasComponent() {
				return registry->has<T>(entt_id);
			}

			bool hasComponent(entt::id_type componentId) {
				entt::id_type types[] = { componentId };
				return registry->runtime_view(std::cbegin(types), std::cend(types)).contains(entt_id);
			}

			template <class T>
			T* getComponent() {
				return hasComponent<T>() ? &registry->get<T>(entt_id) : NULL;
			}
			
		};
	}
}

#endif