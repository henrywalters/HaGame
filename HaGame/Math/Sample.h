#ifndef SAMPLE_H
#define SAMPLE_H

#include "../Utils/Aliases.h"

namespace hagame {
	namespace math {

		template <class T>
		struct StatisticalSummary {
			int N;
			T average;
			T stddev;
			T max;
			T min;

			String toString() {
				return "Sample size = " + std::to_string(N) + "\nAverage = " + std::to_string(average) + "\nStdDev = " + std::to_string(stddev) + "\nMin = " + std::to_string(min) + "\nMax = " + std::to_string(max);
			}
		};

		// Provides a set with a fixed size (N) of type T and functions for common mathematical formulas. The onFull callback is triggered when the set is full.
		template<class T, size_t N>
		class Sample {
		private:

			Queue<T> set;

		public:
			
			std::function<void()> onFull;
			size_t size;

			Sample() : size(N), onFull([]() {}) {}
			Sample(std::function<void()> _onFull) : size(N), onFull(_onFull) {}

			void insert(T value) {
				if (set.size() == N && set.size() > 0) {
					onFull();
					set.pop_front();
				}
				set.push_back(value);
			}

			void clear() {
				set.clear();
			}

			T max() {
				if (size == 0)
					return 0;
				T m = set[0];

				for (int i = 1; i < size; i++) {
					if (set[i] > m)
						m = set[i];
				}

				return m;
			}

			T min() {
				if (size == 0)
					return 0;
				T m = set[0];

				for (int i = 1; i < size; i++) {
					if (set[i] < m)
						m = set[i];
				}

				return m;
			}

			T sum() {
				T s = 0;
				for (T el : set) {
					s += el;
				}
				return s;
			}

			T average() {
				return sum() / size;
			}

			T stddev() {
				T avg = average();
				T s = 0;
				for (T el : set) {
					T x = el - avg;
					s += x * x;
				}
				return sqrt(s / size);
			}

			StatisticalSummary<T> summary() {
				StatisticalSummary<T> res;
				res.N = size;
				res.average = average();
				res.stddev = stddev();
				res.min = min();
				res.max = max();
				return res;
			}
		};
	}
}

#endif
