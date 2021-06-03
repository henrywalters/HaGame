#ifndef TESTING_H
#define TESTING_H
#include "../Math/Sample.h"
#include "./Timer.h"
#include "Aliases.h"

// A set of utilities to make testing a bit easier
namespace hagame {
	namespace utils {

		template <class T, size_t SampleSize>
		hagame::math::StatisticalSummary<T> testFunctionSpeed(voidfunc_t func) {
			hagame::math::Sample<T, SampleSize> sample;
			Timer timer;

			for (int i = 0; i < SampleSize; i++) {
				timer.reset();
				func();
				sample.insert(timer.elapsed());
			}

			return sample.summary();
		}
	}
}

#endif