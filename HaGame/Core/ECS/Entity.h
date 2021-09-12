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

			Set<String> tags;

			entt::basic_registry<uint32_t>* registry;

			Ptr<Entity> parent;
			Array<Ptr<Entity>> children;

			Ptr<Transform> transform;

			uint32_t entt_id;
			uint32_t id;
			std::string name;

			Entity(uint32_t _entt_id, uint32_t _id) : id(_id), entt_id(_entt_id) {
				name = "Entity_" + std::to_string(_id);
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

			template <class T>
			void removeComponent() {
				registry->remove_if_exists<T>(entt_id);
			}

			template <class T>
			bool hasComponent() {
				return registry->has<T>(entt_id);
			}

			template <class T>
			T* getComponent() {
				return hasComponent<T>() ? &registry->get<T>(entt_id) : NULL;
			}
			
		};
	}
}

#endif