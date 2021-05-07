#ifndef SAMPLE_H
#define SAMPLE_H

#include "../Utils/Aliases.h"

namespace hagame {
	namespace math {

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
				if (set.size() == N) {
					onFull();
					set.pop_front();
				}
				set.push_back(value);
			}

			void clear() {
				set.clear();
			}

			T sum() {
				T s = 0;
				for (T el : set) {
					s += el;
				}
				return s;
			}

			T average() {
				return sum() / N;
			}

			T stddev() {
				T avg = average();
				T s = 0;
				for (T el : set) {
					T x = el - avg;
					s += x * x;
				}
				return sqrt((s * s) / N);
			}
		};
	}
}

#endif
