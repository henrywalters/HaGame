#ifndef ALIASES
#define ALIASES

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "../Math/Vector.h"
#include "../Math/Hypercube.h"

// Standard Library aliases
using String = std::string;
using Exception = std::exception;

template <class T>
using Ptr = std::shared_ptr<T>;

template <class T>
using UniqPtr = std::unique_ptr<T>;

template <class T>
using Array = std::vector<T>;

template <class K, class V>
using Map = std::unordered_map<K, V>;

template <class K, class V>
bool hasKey(Map<K, V> map, std::string key) {
	return map.find(key) != map.end();
}

template <class T>
using Queue = std::deque<T>;

// Math aliases
const float PI = 3.141592f;
const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;

// Vector aliases
using Vec4 = hagame::math::Vector<4, float>;
using Vec4Int = hagame::math::Vector<4, int>;
using Vec4UInt32 = hagame::math::Vector<4, uint32_t>;

using Vec3 = hagame::math::Vector <3, float>;
using Vec3Int = hagame::math::Vector<3, int>;
using Vec3UInt32 = hagame::math::Vector<3, uint32_t>;

using Vec2 = hagame::math::Vector<2, float>;
using Vec2Int = hagame::math::Vector<2, int>;
using Vec2UInt32 = hagame::math::Vector<2, uint32_t>;

using Rect = hagame::math::Hypercube<2, float>;
using RectInt = hagame::math::Hypercube<2, int>;
using RectUInt32 = hagame::math::Hypercube<2, uint32_t>;
using Cube = hagame::math::Hypercube<3, float>;
using CubeInt = hagame::math::Hypercube<3, int>;
using CubeUInt32 = hagame::math::Hypercube<3, uint32_t>;

// Simple helpers

inline std::string glMat4ToString(glm::mat4 mat) {
	std::string out = "";
	const float* matSrc = (const float*)glm::value_ptr(mat);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			out += std::to_string(matSrc[j * 4 + i]) + " ";
		}
		out += "\n";
	}
	return out;
}

inline Vec2 hgVec2(glm::vec2 vec) {
	return Vec2({ vec[0], vec[1] });
}

inline glm::vec2 glVec2(Vec2 vec) {
	return glm::vec2(vec[0], vec[1]);
}

inline Vec3 hgVec3(glm::vec3 vec) {
	return Vec3({ vec[0], vec[1], vec[2] });
}

inline glm::vec3 glVec3(Vec3 vec) {
	return glm::vec3(vec[0], vec[1], vec[2]);
}

inline glm::vec4 glVec4(Vec4 vec) {
	return glm::vec4(vec[0], vec[1], vec[2], vec[3]);
}

#endif