#ifndef SCENE_H
#define SCENE_H

#include <cstdint>
#include "ECS/System.h"
#include "ECS/EntityManager.h"
#include "../Utils/Aliases.h"
#include "../Utils/StateManager.h"
#include "Game.h"
#include "../Vendor/entt/entt.hpp"

namespace hagame {
	class Scene {
	private:

		friend class Game;
		
		entt::basic_registry<uint32_t> _registry;

		virtual void onSceneActivate() {}
		virtual void onSceneUpdate(double dt) {}
		virtual void onSceneDeactivate() {}

		void activate() {
			for (auto system : systems) {
				system.start();
			}

			onSceneActivate();
		}

		void deactivate() {
			for (auto system : systems) {
				system.stop();
			}

			onSceneDeactivate();
		}

		void update(double dt) {
			for (auto system : systems) {
				system.update(dt);
			}

			onSceneUpdate(dt);
		}

	public:

		Array<ecs::System> systems;
		ecs::EntityManager entities;

		Scene() : entities(&_registry) {}

		entt::basic_registry<uint32_t>* getRegistry() {
			return &_registry;
		}
	};
}

#endif
