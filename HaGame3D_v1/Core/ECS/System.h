#ifndef SYSTEM_H
#define SYSTEM_H

namespace hagame {
	namespace ecs {
		class System {
		public:
			friend class Scene;
			virtual void onSystemStart() {}
			virtual void onSystemUpdate(double dt) {}
			virtual void onSystemStop() {}

			void start() {
				onSystemStart();
			}

			void stop() {
				onSystemStop();
			}

			void update(double dt) {
				onSystemUpdate(dt);
			}
		};
	}
}

#endif 