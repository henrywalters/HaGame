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
			Map<uint32_t, UniqPtr<Entity>> enttMap;
			Map<uint32_t, RawPtr<Entity>> idMap;
			int entityCount = 0;

			void removeChild(RawPtr<Entity> parent, RawPtr<Entity> child) {
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
			/*
			EntityIterator begin() { 
				return EntityIterator(&enttMap.begin()->second);
			}

			EntityIterator end() {
				return EntityIterator(&enttMap.end()->second);
			}
			*/
			
			RawPtr<Entity> add() {
				auto entityId = registry->create();
				enttMap[entityId] = std::make_unique<Entity>(entityId, entityCount);
				//Ptr<Entity> entity = std::make_shared<Entity>(registry->create(), entityCount);
				entityCount++;
				enttMap[entityId]->registry = registry;
				enttMap[entityId]->transform = std::make_shared<hagame::Transform>();
				enttMap[entityId]->transform->entity = enttMap[entityId].get();
				idMap[entityId] = enttMap[entityId].get();
				return enttMap[entityId].get();
			}

			void forEach(std::function<void(RawPtr<Entity>)> lambda) {
				for (auto& [id, entity] : enttMap) {
					if (entity == nullptr) continue;
					lambda(entity.get());
				}
			}

			void forEach(std::function<void(RawPtr<Entity>)> lambda, Array<uint64_t> ignoreEntities, Array<String> ignoreTags) {
				for (auto& [id, entity] : enttMap) {
					if (entity == nullptr) continue;
					if (!hasElement(ignoreEntities, (uint64_t)entity->uuid) && !entity->hasTag(ignoreTags)) {
						lambda(entity.get());
					}
					
				}
			}

			template <class T>
			void forEach(std::function<void(RawPtr<T>, RawPtr<Entity>)> lambda) {
				for (auto entity : registry->view<T>()) {
					lambda(&registry->get<T>(entity), getByEnttId(entity));
				}
			}

			template <class T>
			void forEach(std::function<void(RawPtr<T>, Ptr<Entity>)> lambda, Array<uint64_t> ignoreEntities, Array<String> ignoreTags) {
				for (auto entity : registry->view<T>()) {
					if (!hasElement(ignoreEntities, (uint64_t)entity->uuid) && !entity->hasTag(ignoreTags)) {
						lambda(&registry->get<T>(entity), getByEnttId(entity));
					}
				}
			}

			// Instantiate a new Entity belonging to the parent
			RawPtr<Entity> add(RawPtr<Entity> parent) {
				RawPtr<Entity> entity = add();
				entity->parent = parent;
				parent->children.push_back(entity);
				entity->transform->setPosition(Vec3::Zero());
				return entity;
			}

			// Add an existing entity to a parent
			RawPtr<Entity> add(RawPtr<Entity> parent, RawPtr<Entity> child) {

				// If the child is currently the parent of the target parent, remove the relationship
				if (parent->parent != NULL && parent->parent->id == child->id) {
					removeChild(parent->parent, parent);
				}

				if (child->parent != NULL) {
					removeChild(child->parent, child);
				}

				child->transform->setPosition(child->transform->getPosition() - parent->transform->getPosition());
				child->parent = parent;
				child->transform->entity = child;
				parent->children.push_back(child);
				return child;
			}

			RawPtr<Entity> get(uint32_t id) {
				return idMap[id];
			}

			RawPtr<Entity> getByEnttId(uint32_t id) {
				return enttMap[id].get();
			}

			// Remove an entity, and its children
			void remove(RawPtr<Entity> entity) {
				for (auto child : entity->children) {
					remove(child);
				}

				if (entity->parent != NULL) {
					removeChild(entity->parent, entity);
				}

				enttMap[entity->entt_id].reset();
			}

			void clear() {
				std::vector<long> parentIds;
				for (auto& [key, entity] : idMap) {
					if (entity->parent == nullptr) {
						parentIds.push_back(entity->entt_id);
					}
				}

				for (auto id : parentIds) {
					remove(enttMap[id].get());
				}

				enttMap.clear();
				idMap.clear();
				registry->clear();

				entityCount = 0;
			}

			template <class T>
			void sortByDistance(Vec3 pos) {
				registry->sort<T>([this, pos](const uint32_t lhs, const uint32_t rhs) {
					return (enttMap[lhs]->transform->getPosition() - pos).magnitude()
						> (enttMap[rhs]->transform->getPosition() - pos).magnitude();
				});
			}

		};
	}
}

#endif