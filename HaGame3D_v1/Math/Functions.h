#ifndef Functions
#define Functions
#include "Vector.h"
#include "../Utils/Aliases.h"

template <class T>
T clamp(T val, T min, T max) {
	if (val >= max) return max;
	else if (val <= min) return min;
	else return val;
}

// Maps a value in the domain of [0, max] to range [0, 1]; Note, the value will be clamped to the bounds.
template <class T>
T normalize(T val, T max) {
	return normalize<T>(val, 0, max);
}

// Maps a value in the domain of [min, max] to range [0, 1]; Note, the value will be clamped to the bounds.
template <class T>
T normalize(T val, T min, T max) {
	return (val - min) / (max - min);
}

// Maps an angle (in radians) to a unit vector
template <class T>
hagame::math::Vector<2, T> getUnitVector(T rads) {
	return hagame::math::Vector<2, T>({ cos(rads), sin(rads) });
}

// Maps a vector to an angle (in radians);
template <class T>
T getAngle(hagame::math::Vector<2, T> vector) {
	return atan2(vector[1], vector[0]);
}

#endif