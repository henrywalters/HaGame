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

			template<class T>
			T normal(T mean, T stdev) {
				std::normal_distribution<T> dist(mean, stdev);
				return dist(generator);
			}

			template<class T>
			T poisson(T mean) {
				std::poisson_distribution<T> dist(mean);
				return dist(generator);
			}

			template<class T>
			T lognorm(T mean, T stddev) {
				std::lognormal_distribution dist(mean, stddev);
				return dist(generator);
			}
		};
	}
}

#endif
