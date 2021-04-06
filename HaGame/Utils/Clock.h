#ifndef CLOCK
#define CLOCK
#include <chrono>

namespace hagame {
	namespace utils {
		class Clock {
		public:
			static long Now() {
				std::chrono::time_point<std::chrono::steady_clock> time = std::chrono::high_resolution_clock::now();
				return std::chrono::duration_cast<std::chrono::microseconds>(time.time_since_epoch()).count();
			}

			static void Wait(long microseconds) {
				long elapsed = 0;
				long start = Clock::Now();
				while (elapsed < microseconds) {
					elapsed = Clock::Now() - start;
				}
			}
		};
	}
}

#endif