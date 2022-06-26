#ifndef Functions
#define Functions
#include "Vector.h"
#include "Line.h"
#include "../Utils/Aliases.h"

template <class T>
T max(T a, T b) {
	return a > b ? a : b;
}

template <class T>
T min(T a, T b) {
	return a < b ? a : b;
}

template <size_t N, class T>
T max(T arr[N]) {
	T max = 0;
	bool first = false;
	for (int i = 0; i < N; i++) {
		if (!first) {
			max = arr[i];
			first = true;
		}
		else if (arr[i] > max) {
			max = arr[i];
		}
	}
	return max;
}

template <size_t N, class T>
T min(T arr[N]) {
	T min = 0;
	bool first = false;
	for (int i = 0; i < N; i++) {
		if (!first) {
			min = arr[i];
			first = true;
		}
		else if (min == NULL || arr[i] < min) {
			min = arr[i];
		}
	}
	return min;
}

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
	if (t > 1.0)
		return b;
		
	if (t < 0.0)
		return a;
	
	return (b - a) * t + a;
}

// Return the sign (-1, 0, 1) of a number
template <class T>
T sign(T a) {
	if (a > 0) {
		return 1;
	}
	else if (a < 0) {
		return -1;
	}
	else {
		return 0;
	}
}

// Return the sign (-1, 0, 1) of a vector
template <class T, size_t N>
T sign(hagame::math::Vector<N, T> vector) {

	T a = vector.magnitude();

	if (a > 0) {
		return 1;
	}
	else if (a < 0) {
		return -1;
	}
	else {
		return 0;
	}
}

// Useful function for mapping [-inf,inf] to (0,1)
template <class T>
T sigmoid(T x) {
	return 1 / (1 + exp(-x));
}

// Compute the barycentric coordinates (u, v, w) for point p with respect to triangle T (ABC)
template <class T>
hagame::math::Vector<3, T> computeBarycentric(hagame::math::Vector<3, T> a, hagame::math::Vector<3, T> b, hagame::math::Vector<3, T> c, hagame::math::Vector<3, T> p) {
	hagame::math::Vector<3, T> v0 = b - a, v1 = c - a, v2 = p - a;

	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);

	float denom = d00 * d11 - d01 * d01;

	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;

	return hagame::math::Vector<3, T>({u, v, w});
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

// Calculate the cells, in an abstract grid, that will be intersected by a line
Array<Vec3> calcCellsThatIntersectLine(Vec3 cellSize, Vec3 origin, Vec3 direction) {
	// Based on this paper: http://www.cse.yorku.ca/~amana/research/grid.pdf

	int MAX_ITERATIONS = 100;
	int iterations = 0;

	Vec3Int offset = Vec3Int({
		(int)(origin[0] / cellSize[0]),
		(int)(origin[1] / cellSize[1]),
		(int)(origin[2] / cellSize[2]),
	});

	Vec3Int targetIdx = Vec3Int({
		(int)((origin[0] + direction[0]) / cellSize[0]) - offset[0],
		(int)((origin[1] + direction[1]) / cellSize[1]) - offset[1],
		(int)((origin[2] + direction[2]) / cellSize[2]) - offset[2],
	});

	Array<Vec3> cells = Array<Vec3>();
	Vec3Int pos;
	Vec3 step = Vec3::Identity();
	Vec3 tMax;
	Vec3 tDelta;

	Vec3Bool flip = Vec3Bool({ direction[0] < 0.0, direction[1] < 0.0, direction[2] < 0.0 });

	for (int i = 0; i < 3; i++) {
		if (flip[i])
			targetIdx[i] *= -1;
		tMax[i] = direction[i] != 0 ? cellSize[i] / std::abs(direction[i]) : 0.0;
	}
	tDelta = tMax.copy();

	cells.push_back(Vec3({
			((flip[0] ? -1 * pos[0] : pos[0]) + offset[0]) * cellSize[0],
			((flip[1] ? -1 * pos[1] : pos[1]) + offset[1]) * cellSize[1],
			((flip[2] ? -1 * pos[2] : pos[2]) + offset[2]) * cellSize[2]
	}));

	std::cout << origin.toString() << std::endl;
	std::cout << direction.toString() << std::endl;
	std::cout << tMax.toString() << std::endl;
	std::cout << targetIdx.toString() << std::endl;

	while (pos != targetIdx && iterations < MAX_ITERATIONS) {

		std::cout << pos.toString() << std::endl;

		if (tMax[0] < tMax[1]) {
			if (tMax[0] < tMax[2]) {
				pos[0] += step[0];
				tMax[0] += tDelta[0];
			}
			else {
				pos[2] += step[2];
				tMax[2] += tDelta[2];
			}
		}
		else {
			if (tMax[1] < tMax[2]) {
				pos[1] += step[1];
				tMax[1] += tDelta[1];
			}
			else {
				pos[2] += step[2];
				tMax[2] += tDelta[2];
			}
		}
		
		cells.push_back(Vec3({
			((flip[0] ? -1 * pos[0] : pos[0]) + offset[0]) * cellSize[0],
			((flip[1] ? -1 * pos[1] : pos[1]) + offset[1]) * cellSize[1],
			((flip[2] ? -1 * pos[2] : pos[2]) + offset[2])* cellSize[2]
		}));

		iterations++;

	}
	
	return cells;

}

template <size_t N, class T>
hagame::math::Vector<N, T> bezier(hagame::math::Vector<N, T> p1, hagame::math::Vector<N, T> p2, hagame::math::Vector<N, T> p3, T t) {
	T tInverse = 1 - t;
	return (tInverse * tInverse * p1) + (tInverse * t * p2) + (t * p3);
}

// Checks if the lines AB and PQ intersect. If so, t and intersection are set relative to AB.
bool linesIntersect(Vec2 a, Vec2 b, Vec2 p, Vec2 q, float& t) {
	Vec2 s1 = b - a;
	Vec2 s2 = q - p;

	float denom = -s2[0] * s1[1] + s1[0] * s2[1];
	
	// Colinear or parallel so fuck it
	if (denom < 0.01) {
		return false;
	}

	float s = (-s1[1] * (a[0] - p[0]) + s1[0] * (a[1] - p[1])) / denom;
	t = (s2[0] * (a[1] - p[1]) - s2[1] * (a[0] - p[0])) / denom;

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
		return true;
	}
	else {
		return false;
	}
}

template <class T>
T mapToDomain(T a, T b, T newA, T newB, T value) {
	float scale = (b - a) / (newB - newA);
	return ((value - a) / scale) + newA;
}

// Convert a world position to a normalized screen position
Vec2 WorldToScreen(Mat4 model, Mat4 view, Mat4 proj, Vec3 worldPos) {
	auto pos = proj * view * model * worldPos.resize<4>(1.0f);
	pos[3] = 1.0f / pos[3];
	for (int i = 0; i < 2; i++) {
		pos[i] *= pos[3];
	}
	return pos.resize<2>();
}

// Convert a normalized [-1:1, -1:1] screen position to a world position
Vec3 ScreenToWorld(Mat4 view, Mat4 proj, Vec2 screen, float depth = 1.0f) {
	auto pvI = (proj * view).inverted();
	auto screen4 = screen.resize<4>();

	screen4[2] = depth;
	screen4[3] = 1.0f;

	auto worldPos = pvI * screen4;
	
	worldPos[3] = 1.0f / worldPos[3];

	for (int i = 0; i < 3; i++) {
		worldPos[i] *= worldPos[3];
	}

	return worldPos.resize<3>();
}

// Convert raw mouse position to a world position
Vec3 MouseToWorld(Mat4 view, Mat4 proj, Vec2 mousePos, Vec2 windowSize, float depth = 1.0f) {
	auto mousePosNorm = Vec2({
		mapToDomain<float>(0.0f, windowSize[0], -1.0f, 1.0f, mousePos[0]),
		-mapToDomain<float>(0.0f, windowSize[1], -1.0f, 1.0f, mousePos[1])
	});
	return ScreenToWorld(view, proj, mousePosNorm, depth);
}

// Convert raw mouse position to normalized [-1:1, -1:1] screen position
Vec2 MouseToScreen(Vec2 windowSize, Vec2 mousePos) {
	return Vec2({
		mapToDomain<float>(0.0f, windowSize[0], -1.0f, 1.0f, mousePos[0]),
		-mapToDomain<float>(0.0f, windowSize[1], -1.0f, 1.0f, mousePos[1])
	});
}

// Construct a uint64_t hash id from an array of other data types
template <class T, size_t N>
uint64_t HashIds(T ids[N]) {
	uint64_t id = 0;

	for (int i = 0; i < N; i++) {
		id *= 37;
		id += ids[i];
	}

	return id;
}

#endif