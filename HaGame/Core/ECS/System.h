#ifndef SYSTEM_H
#define SYSTEM_H

#include <cstdint>
#include <string>
#include "../../Vendor/entt/entt.hpp"
#include "../../Utils/Timer.h"
#include "../../Utils/Aliases.h"

namespace hagame {

	class Game;
	class Scene;

	namespace ecs {

		class Entity;

		class System {
		private:
			friend class SystemManager;
		protected:
			hagame::Game* game;
			Scene* scene;
			String name;
			entt::basic_registry<uint32_t>* registry;
			utils::Timer timer;

		public:

			bool active = true;

			virtual String getSystemName() = 0;

			virtual void onSystemInit() {}
			virtual void onSystemStart() {}
			virtual void onSystemBeforeUpdate(double dt) {}
			virtual void onSystemUpdate(double dt) {}
			virtual void onSystemPhysicsBeforeUpdate(double dt) {}
			virtual void onSystemPhysicsUpdate(double dt) {}
			virtual void onSystemPhysicsAfterUpdate(double dt) {}
			virtual void onSystemAfterUpdate(double dt) {}
			virtual void onSystemStop() {}

			template <class T>
			void forEach(std::function<void(T*)> lambda) {
				for (auto entity : registry->view<T>()) {
					lambda(&registry->get<T>(entity));
				}
			}

			template <class T>
			void forEach(std::function<void(RawPtr<T>, RawPtr<Entity>)> lambda) {
				for (auto entity : registry->view<T>()) {
					lambda(&registry->get<T>(entity), scene->ecs.entities.getByEnttId(entity));
				}
			}

			void init() {
				onSystemInit();
			}

			void start() {
				if (active) {
					timer.reset();
					onSystemStart();
				}
			}

			void stop() {
				if (active) {
					onSystemStop();
				}
			}

			void update(double dt) {
				if (active) {
					onSystemUpdate(dt);
				}
			}

			void physicsUpdate(double dt) {
				if (active) {
					onSystemPhysicsUpdate(dt);
				}
			}

			void physicsBeforeUpdate(double dt) {
				if (active) {
					onSystemPhysicsBeforeUpdate(dt);
				}
			}

			void physicsAfterUpdate(double dt) {
				if (active) {
					onSystemPhysicsAfterUpdate(dt);
				}
			}

			void beforeUpdate(double dt) {
				if (active) {
					onSystemBeforeUpdate(dt);
				}
			}

			void afterUpdate(double dt) {
				if (active) {
					onSystemAfterUpdate(dt);
				}
			}
		};
	}
}

#endif 