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

		class System {
		private:
			friend class SystemManager;
		protected:
			Game* game;
			Scene* scene;
			String name;
			entt::basic_registry<uint32_t>* registry;
			utils::Timer timer;

		public:

			bool active = true;

			virtual String getSystemName() = 0;
			virtual void onSystemStart() {}
			virtual void onSystemBeforeUpdate() {}
			virtual void onSystemUpdate(double dt) {}
			virtual void onSystemAfterUpdate() {}
			virtual void onSystemStop() {}

			void start() {
				if (active) {
					timer.reset();
					onSystemStart();
					DEBUG_LOG_VAR(timer.elapsed());
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

			void beforeUpdate() {
				if (active) {
					onSystemBeforeUpdate();
				}
			}

			void afterUpdate() {
				if (active) {
					onSystemAfterUpdate();
				}
			}
		};
	}
}

#endif 