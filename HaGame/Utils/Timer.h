#ifndef TIMER_H
#define TIMER_H
#include "./Aliases.h"
#include "./Clock.h"

namespace hagame {
	namespace utils {

		// Record the amount of time that has passed
		class Timer {
		public:
			long _start;

			Timer(): _start(Clock::Now()) {}

			void reset() {
				_start = Clock::Now();
			}

			long elapsed() {
				return Clock::Now() - _start;
			}

		};
	}
}

#endif