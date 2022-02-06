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

			template <class T, class... Args>
			T* addComponent(Args &&... args) {
				T* component = &registry->emplace<T>(entt_id, std::forward<Args>(args)...);
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

			template <class T>
			T* getOrCreateComponent() {
				return hasComponent<T>() ? &registry->get<T>(entt_id) : addComponent<T>();
			}
			
			/*
			Vec3 getPos() { return transform->getPosition(); }
			Quat getRot() { return transform->getRotation(); }
			Vec3 getScale() { return transform->getScale(); }

			void setPos(Vec3 pos) { transform->setPosition(pos); }
			void move(Vec3 delta) { transform->move(delta); }
			void setRot(Quat rot) { transform->setRotation(rot); }
			void rotate(Quat delta) { transform->rotate(delta); }
			void setScale(Vec3 scale) { transform->setScale(scale); }
			*/
		};

		struct EntityIterator {
			using iterator_category = std::input_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Ptr<Entity>;
			using pointer = Ptr<Entity>*;
			using reference = Ptr<Entity>&;

			EntityIterator(pointer ptr) : m_ptr(ptr) {}

			reference operator*() const { return *m_ptr; }
			pointer operator->() { return m_ptr; }

			EntityIterator& operator++() { m_ptr++; return *this; }
			EntityIterator operator++(int) { EntityIterator tmp = *this; ++(*this); return tmp; }

			friend bool operator==(const EntityIterator& a, const EntityIterator& b) { return (*a.m_ptr)->uuid == (*b.m_ptr)->uuid; }
			friend bool operator!=(const EntityIterator& a, const EntityIterator& b) { return (*a.m_ptr)->uuid != (*b.m_ptr)->uuid; }
		private:
			pointer m_ptr;
		};
	}
}

#endif