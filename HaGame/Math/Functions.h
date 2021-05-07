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

// Linearly Interpolates a value between [a, b]. Does not clamp the value.
template <class T>
T lerp(T a, T b, T t) {
	return (b - a) * t + a;
}

// Linearly Interpolates a value between [a, b]. Clamps the value between a and b.
template <class T>
T lerpClamp(T a, T b, T t) {
	if t > 1.0 return b;
	if t < 0.0 return a;
	return (b - a) * t + a;
}

// Adjusts a box (assumed to be 3d axis-aligned, such as a Mesh Model Bounding Box) by a transform matrix and translation vector. 
template <size_t dim, class T>
hagame::math::Hypercube<dim, T> transformBoundingBox(hagame::math::Hypercube<dim, T> box, hagame::math::Matrix<dim, dim, T> transform, hagame::math::Vector<dim, T> translation) {
	// Implementation based on Jim Avro's code: http://www.realtimerendering.com/resources/GraphicsGems/gems/TransBox.c

	float a, b;
	hagame::math::Vector<dim, T> aMin, aMax, bMin, bMax;
	int i, j;

	aMin = box.min().copy();
	aMax = box.max().copy();

	bMin = translation.copy();
	bMax = bMin.copy();

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			a = transform.get(i, j) * aMin[j];
			b = transform.get(i, j) * aMax[j];

			if (a < b) {
				bMin[i] += a;
				bMax[i] += b;
			}
			else {
				bMin[i] += b;
				bMax[i] += a;
			}
		}
	}

	return hagame::math::Hypercube(bMin, bMax - bMin);
}

#endif