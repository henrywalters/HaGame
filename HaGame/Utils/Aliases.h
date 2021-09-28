#ifndef ALIASES
#define ALIASES

#include <any>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <set>
#include <stack>
#include <tuple>
#include <memory>
#include <optional>
#include <list>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "../Math/Vector.h"
#include "../Math/Matrix.h"
#include "../Math/Hypercube.h"
#include "../Math/NSphere.h"
#include "./../Vendor/json.hpp"

#ifndef DEBUG_MODE
#define DEBUG_MODE false
#endif 

#ifndef DEBUG_LOG_VAR
#define DEBUG_LOG_VAR(x) std::cout << #x " = " << x << std::endl;
#endif

#ifndef DEBUG_LOG
#define DEBUG_LOG(key, value) std::cout << key << " = " << value << std::endl;
#endif

typedef void (*voidfunc_t)();


// Override this to provide your own custom data structures
#ifndef HAGAME_STD_ALIASES
#define HAGAME_STD_ALIASES

// Standard Library aliases

using Any = std::any;

template <class T>
using Optional = std::optional<T>;

using String = std::string;
using Exception = std::exception;

template <class T>
using Ptr = std::shared_ptr<T>;

template <class T>
using UniqPtr = std::unique_ptr<T>;

template <class T>
using Array = std::vector<T>;

template <class T>
using List = std::list<T>;

#endif

template <class T>
bool hasElement(Array<T> arr, T el) {
	for (T item : arr) {
		if (item == el) return true;
	}
	return false;
}

template <class T>
using Set = std::set<T>;

template <class T>
bool hasKey(Set<T> set, T key) {
	return set.find(key) != set.end();
}

#ifndef toStr
#define toStr(value) std::to_string(value)
#endif

template <class T>
std::string printArray(Array<T> arr) {
	std::string out = "[\n";
	for (int i = 0; i < arr.size(); i++) {
		out += "\t" + std::to_string(arr[i]) + ",\n";
	}
	out += "]";
	return out;
}

template <class K, class V>
using Map = std::unordered_map<K, V>;

template <class K, class V>
bool hasKey(Map<K, V> map, K key) {
	return map.find(key) != map.end();
}

template <class K, class V>
Map<K, Any> make_any(Map<K, V> map) {
	Map<K, Any> out;
	for (auto& [key, value] : map) {
		out.insert(std::make_pair(key, value));
	}
	return out;
}

template <class T>
using Queue = std::deque<T>;

using JSON = nlohmann::json;

// Math aliases
const float EPSILON = 0.1f;
const float E = 2.7182818f;
const float PI = 3.141592f;
const float SQRT_2 = 1.414213;
const float SQRT_3 = 1.732051;
const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;

// Vector aliases
using Vec4 = hagame::math::Vector<4, float>;
using Vec4Int = hagame::math::Vector<4, int>;
using Vec4UInt32 = hagame::math::Vector<4, uint32_t>;

using Vec3 = hagame::math::Vector <3, float>;
using Vec3UInt = hagame::math::Vector <3, unsigned int>;
using Vec3Int = hagame::math::Vector<3, int>;
using Vec3UInt32 = hagame::math::Vector<3, uint32_t>;

using Vec2 = hagame::math::Vector<2, float>;
using Vec2Int = hagame::math::Vector<2, int>;
using Vec2UInt32 = hagame::math::Vector<2, uint32_t>;

using Vec3Bool = hagame::math::Vector<3, bool>;

using Rect = hagame::math::Hypercube<2, float>;
using RectInt = hagame::math::Hypercube<2, int>;
using RectUInt32 = hagame::math::Hypercube<2, uint32_t>;
using Cube = hagame::math::Hypercube<3, float>;
using CubeInt = hagame::math::Hypercube<3, int>;
using CubeUInt32 = hagame::math::Hypercube<3, uint32_t>;

using Circle = hagame::math::NSphere<2, float>;
using Sphere = hagame::math::NSphere<3, float>;

// Vector functions

// The normalized version of vec
inline Vec3 normalize(Vec3 vec) {
	return vec.normalized();
}

// Returns the dot product of a and b
inline float dot(Vec3 a, Vec3 b) {
	return a.dot(b);
}

// Returns dot(a, a)
inline float dot2(Vec3 a) {
	return a.dot(a);
}

// Returns the cross product of a and b
inline Vec3 cross(Vec3 a, Vec3 b) {
	return a.cross(b);
}

// Matrix aliases
using Mat4 = hagame::math::Matrix<4, 4, float>;

using Mat3 = hagame::math::Matrix<3, 3, float>;

using Quat = hagame::math::Quaternion<float>;

inline Rect cubeToRect(Cube cube) {
	return Rect(Vec2({ cube.pos[0], cube.pos[1] }), Vec2({ cube.size[0], cube.size[1] }));
}

// Simple helpers

inline std::string glVec3ToString(glm::vec3 vec3) {
	return "[" + std::to_string(vec3[0]) + ", " + std::to_string(vec3[1]) + ", " + std::to_string(vec3[2]) + "]";
}

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

template <template <typename...> class Base, typename Derived>
struct is_base_of_template
{
	using U = typename std::remove_cv<Derived>::type;

	template <typename... Args>
	static std::true_type test(Base<Args...>*);

	static std::false_type test(void*);

	using type = decltype(test(std::declval<U*>()));
};

template <template <typename...> class Base, typename Derived>
using is_base_of_template_t = typename is_base_of_template<Base, Derived>::type;

#endif