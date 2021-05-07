#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace hagame {
	namespace utils {
		class Random {
			std::random_device device;
			std::mt19937 generator;

		public:

			Random() : generator(device()) {}

			template<class T>
			T real(T min, T max) {
				std::uniform_real_distribution<T> dist(min, max);
				return dist(generator);
			}

			template<class T>
			T integer(T min, T max) {
				std::uniform_int_distribution<T> dist(min, max);
				return dist(generator);
			}
		};
	}
}

#endif
