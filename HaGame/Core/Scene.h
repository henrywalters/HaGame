#ifndef SCENE_H
#define SCENE_H

#include "../Utils/Aliases.h"
#include "../Utils/Timer.h"
#include "../Utils/StateManager.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Components/CameraComponent.h"
#include "ECS/System.h"
#include "ECS/Entity.h"
#include "ECS/ECS.h"

namespace hagame {

	class Game;

	class Scene {
	private:
		void setActiveCamera() {

			bool foundCamera = false;
			
			for (auto entity : ecs.getRegistry()->view<graphics::CameraComponent>()) {
				auto cam = &ecs.getRegistry()->get<graphics::CameraComponent>(entity);
				if (cam->active) {
					activeCamera = cam->camera;
					
					activeCameraEntity = ecs.entities.getByEnttId(entity);
					foundCamera = true;
					break;
				}
			}

			if (foundCamera) {
				viewMat = activeCamera->getViewMatrix(activeCameraEntity->transform.get());
				projMat = activeCamera->getProjMatrix(activeCameraEntity->transform.get());
			}
			
		}

	protected:
		friend class Game;

		virtual void onSceneInit() {}
		virtual void onSceneBeforeActivate() {}
		virtual void onSceneActivate() {}
		virtual void onSceneAfterActivate() {}
		virtual void onSceneUpdate(double dt) {}
		virtual void onSceneDeactivate() {}

		void initialize() {
			onSceneInit();
			ecs.systems.initAll();
		}

		void activate() {
			timer.reset();
			onSceneBeforeActivate();
			ecs.systems.startAll();
			onSceneActivate();
			onSceneAfterActivate();
			//DEBUG_LOG("Scene start", timer.elapsed());
		}

		void deactivate() {
			timer.reset();
			ecs.systems.stopAll();
			onSceneDeactivate();
			//DEBUG_LOG("Scene end", timer.elapsed());
		}

		void update(double dt) {
			timer.reset();
			setActiveCamera();
			ecs.systems.beforeUpdateAll(dt);
			ecs.systems.updateAll(dt);
			onSceneUpdate(dt);
			ecs.systems.afterUpdateAll(dt);
			//DEBUG_LOG("Scene update", timer.elapsed());
		}

	public:
		Game* game;
		String name;
		utils::Timer timer;

		ecs::ECS ecs;
		Ptr<ecs::Entity> activeCameraEntity;
		Ptr<graphics::Camera> activeCamera;
		Mat4 viewMat;
		Mat4 projMat;
		Mat4 uiProjMat;

		template<class T>
		Ptr<T> addSystem() {
			return ecs.systems.add<T>(game, this);
		}

		template<class T>
		Ptr<T> getSystem() {
			return ecs.systems.get<T>(game, this);
		}

		Ptr<ecs::Entity> addChild(Ptr<ecs::Entity> parent) {
			return ecs.entities.add(parent);
		}

		Ptr<ecs::Entity> addEntity() {
			return ecs.entities.add();
		}

		void removeEntity(Ptr<ecs::Entity> entity) {
			ecs.entities.remove(entity);
		}
	};
}

#endif
