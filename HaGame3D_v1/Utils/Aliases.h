#ifndef ALIASES
#define ALIASES

#include <vector>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "../Math/Vector.h"

using Vec3 = hagame::math::Vector < 3, float >;
inline Vec3 hgVec3(glm::vec3 vec) {
	return Vec3({ vec[0], vec[1], vec[2] });
}
inline glm::vec3 glVec3(Vec3 vec) {
	return glm::vec3(vec[0], vec[1], vec[2]);
}

using Vec3Int = hagame::math::Vector<3, int>;
using Vec3UInt32 = hagame::math::Vector<3, uint32_t>;

using Vec2 = hagame::math::Vector<2, float>;
using Vec2Int = hagame::math::Vector<2, int>;
using Vec2UInt32 = hagame::math::Vector<2, uint32_t>;

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


#endif