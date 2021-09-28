#ifndef ECS_H
#define ECS_H

#include <cstdint>
#include "SystemManager.h"
#include "EntityManager.h"
#include "../../Vendor/entt/entt.hpp"
#include "./Entity.h"

namespace hagame {
	class Game;
}

template <class Component>
void ComponentEditorWidget(hagame::Game* game, Ptr<hagame::ecs::Entity> entity) {};


template <class Component>
void ComponentAdd(hagame::Game* game, Ptr<hagame::ecs::Entity> entity) {
	game->scenes.active()->ecs.getRegistry()->emplace<Component>(entity->entt_id);
}

namespace hagame {
	namespace ecs {

		using EntityType = uint32_t;

		struct ComponentInfo {
			using Callback = std::function<void(Game*, Ptr<Entity>)>;
			String name;
			Callback widget;
			Callback add;
		};

		/*
		* @brief The ECS class provides a single set of entities and systems on top of the entt registry. See the wiki for more details: https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#types-const-non-const-and-all-in-between for more details.
		*/ 
		class ECS {
		private:
			entt::basic_registry<EntityType> registry;
			
		public:
			Map<entt::id_type, Ptr<ComponentInfo>> componentInfo;
			EntityManager entities;
			SystemManager systems;

			ECS(): entities(EntityManager(&registry)), systems(SystemManager(&registry)) {}
			~ECS() {}

			entt::basic_registry<EntityType>* getRegistry() {
				return &registry;
			}

			template <class T>
			Ptr<ComponentInfo> registerComponent(String name, typename ComponentInfo::Callback widget) {
				auto index = entt::type_hash<T>::value();
				auto info = std::make_shared<ComponentInfo>();
				info->widget = widget;
				info->add = ComponentAdd<T>;
				info->name = name;
				componentInfo.insert(std::make_pair(index, info));
				return info;
			}

			template <class T>
			Ptr<ComponentInfo> registerComponent(String name) {
				return registerComponent<T>(name, ComponentEditorWidget<T>);
			}
		};
	}
}

#endif
