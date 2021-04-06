#ifndef SCENE_H
#define SCENE_H

#include "../Utils/Aliases.h"
#include "../Utils/StateManager.h"
#include "../Graphics/Camera.h"
#include "ECS/System.h"
#include "ECS/ECS.h"

namespace hagame {

	class Game;

	class Scene {
		friend class Game;

		Game* game;

		virtual void onSceneActivate() {}
		virtual void onSceneUpdate(double dt) {}
		virtual void onSceneDeactivate() {}

		void activate() {
			ecs.systems.startAll();
			onSceneActivate();
		}

		void deactivate() {
			ecs.systems.stopAll();
			onSceneDeactivate();
		}

		void update(double dt) {
			ecs.systems.updateAll(dt);
			onSceneUpdate(dt);
		}

	public:
		ecs::ECS ecs;
		graphics::Camera* activeCamera;

		template<class T>
		ecs::System* addSystem() {
			return ecs.systems.add<T>(game, this);
		}

		ecs::Entity* addEntity() {
			return ecs.entities.add();
		}
	};
}

#endif
