#ifndef SCENE_H
#define SCENE_H

#include "../Utils/Aliases.h"
#include "../Utils/Timer.h"
#include "../Utils/StateManager.h"
#include "../Graphics/Camera.h"
#include "ECS/System.h"
#include "ECS/ECS.h"

namespace hagame {

	class Game;

	class Scene {
		friend class Game;

		Game* game;
		String name;
		utils::Timer timer;

		virtual void onSceneActivate() {}
		virtual void onSceneUpdate(double dt) {}
		virtual void onSceneDeactivate() {}

		void activate() {
			timer.reset();
			onSceneActivate();
			ecs.systems.startAll();
			//DEBUG_LOG("Scene start", timer.elapsed());
		}

		void deactivate() {
			timer.reset();
			onSceneDeactivate();
			ecs.systems.stopAll();
			//DEBUG_LOG("Scene end", timer.elapsed());
		}

		void update(double dt) {
			timer.reset();
			ecs.systems.beforeUpdateAll();
			ecs.systems.updateAll(dt);
			onSceneUpdate(dt);
			ecs.systems.afterUpdateAll();
			//DEBUG_LOG("Scene update", timer.elapsed());
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
