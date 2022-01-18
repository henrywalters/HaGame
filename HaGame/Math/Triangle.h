#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "./Vector.h"
#include "./Functions.h"
#include "../Utils/Aliases.h"
#include "../Utils/String.h"

namespace hagame {
	namespace math {

		struct Triangle {

			Vec3 a;
			Vec3 b;
			Vec3 c;

			Triangle(Vec3 _a, Vec3 _b, Vec3 _c) : a(_a), b(_b), c(_c) {}

			Vec3 getBarycentricCoords(Vec3 point) {
				return computeBarycentric(a, b, c, point);
			}

			bool containsPoint(Vec3 point) {
				Vec3 barycentric = getBarycentricCoords(point);
				return barycentric[1] >= 0.0f && barycentric[2] >= 0.0f && (barycentric[1] + barycentric[2]) <= 1.0f;
			}

			String toString() {
				return stringJoin({ a.toString(), b.toString(), c.toString() }, " , ");
			}

			bool operator==(const Triangle& tri) {
				if (a != tri.a && a != tri.b && a != tri.c)
					return false;
				if (b != tri.a && b != tri.b && b != tri.c)
					return false;
				if (c != tri.a && c != tri.b && c != tri.c)
					return false;
				return true;
			}

			Vec3 calcNormal() {
				return cross(b - a, c - a).normalized();
			}

			Vec3 calcCentroid() {
				return (a + b + c) / 3.0f;
			}

			static Triangle Equilateral(Vec3 pos, float radius) {
				return Triangle(pos + Vec3({ 0, radius, 0 }), pos + Vec3({ radius, -radius, 0 }), pos + Vec3({ -radius, -radius, 0 }));
			}

			Triangle transformed(Mat4 transform) {
				return Triangle(
					(transform * a.resize<4>(1.0f)).resize<3>(),
					(transform * b.resize<4>(1.0f)).resize<3>(),
					(transform * c.resize<4>(1.0f)).resize<3>()
				);
			}
		};

		void to_json(JSON& json, const Triangle& tri) {
			json = { tri.a, tri.b, tri.c };
		}

		void from_json(const JSON& json, Triangle& tri) {
			json.at(0).get_to(tri.a);
			json.at(1).get_to(tri.b);
			json.at(2).get_to(tri.c);
		}
	}
}

#endif
