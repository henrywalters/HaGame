#ifndef TIMED_CALLBACK_H
#define TIMED_CALLBACK_H


namespace hagame {
	namespace utils {

		template <bool Async>
		Ptr<Promise<long, String, Async>> TimedCallback(double delay) {
			long start = Clock::Now();
			return std::make_shared<Promise<long, String, Async>>([start, delay](auto resolve, auto reject) {

				if (delay < 0) reject("Delay must be greater than 0");

				if (Async) {
					while ((Clock::Now() - start) / 1000000 < delay) {}
					resolve(Clock::Now());
				}
				else {
					if ((Clock::Now() - start) / 1000000 >= delay) {
						resolve(Clock::Now());
					}
				}
			});
		}
	}
}

#endif
