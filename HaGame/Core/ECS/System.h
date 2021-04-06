#ifndef SYSTEM_H
#define SYSTEM_H

#include <cstdint>
#include <string>
#include "../../Vendor/entt/entt.hpp"

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
			entt::basic_registry<uint32_t>* registry;
		public:

			bool active = true;

			virtual void onSystemStart() {}
			virtual void onSystemUpdate(double dt) {};
			virtual void onSystemStop() {}

			void start() {
				if (active) {
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
		};
	}
}

#endif 