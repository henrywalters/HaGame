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

			void removeChild(Ptr<Entity> parent, Ptr<Entity> child) {
				for (int i = 0; i < parent->children.size(); i++) {
					if (parent->children[i]->id == child->id) {
						parent->children.erase(parent->children.begin() + i);
						child->transform->move(parent->transform->getPosition());
						child->parent = NULL;
						return;
					}
				}
			}

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

			void forEach(std::function<void(Ptr<Entity>)> lambda) {
				for (auto& [id, entity] : enttMap) {
					lambda(entity);
				}
			}

			// Instantiate a new Entity belonging to the parent
			Ptr<Entity> add(Ptr<Entity> parent) {
				Ptr<Entity> entity = add();
				entity->parent = parent;
				parent->children.push_back(entity);
				entity->transform->setPosition(parent->transform->getPosition());
				return entity;
			}

			// Add an existing entity to a parent
			Ptr<Entity> add(Ptr<Entity> parent, Ptr<Entity> child) {

				// If the child is currently the parent of the target parent, remove the relationship
				if (parent->parent != NULL && parent->parent->id == child->id) {
					removeChild(parent->parent, parent);
				}

				if (child->parent != NULL) {
					removeChild(child->parent, child);
				}

				child->transform->setPosition(child->transform->getPosition() - parent->transform->getPosition());
				child->parent = parent;
				parent->children.push_back(child);
				return child;
			}

			Ptr<Entity> get(uint32_t id) {
				return idMap[id];
			}

			Ptr<Entity> getByEnttId(uint32_t id) {
				return enttMap[id];
			}

			// Remove an entity, and its children
			void remove(Ptr<Entity> entity) {
				for (auto child : entity->children) {
					child->parent.reset();
					remove(child);
				}

				if (entity->parent != NULL) {
					removeChild(entity->parent, entity);
				}

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