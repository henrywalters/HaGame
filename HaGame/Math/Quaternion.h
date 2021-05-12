#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector.h"
#include <glm/mat3x3.hpp>

namespace hagame {
	namespace math {

		template <class T>
		class Quaternion : public Vector<4, T> {
		private:


		public:

			Quaternion() {
				Quaternion(0, Vector<3, T>({ 0, 0, 0 }));
			}

			Quaternion(T rotation, Vector<3, T> axisOfRotation) {
				vector[0] = cos(0.5 * rotation);
				vector[1] = axisOfRotation[0] * sin(0.5 * rotation);
				vector[2] = axisOfRotation[1] * sin(0.5 * rotation);
				vector[3] = axisOfRotation[2] * sin(0.5 * rotation);
			}

			Quaternion(T w, T i, T j, T k) {
				vector[0] = w;
				vector[1] = i;
				vector[2] = j;
				vector[3] = k;
			}

			Quaternion(T roll, T pitch, T yaw) {
				T cy = cos(yaw * 0.5);
				T sy = sin(yaw * 0.5);
				T cp = cos(pitch * 0.5);
				T sp = sin(pitch * 0.5);
				T cr = cos(roll * 0.5);
				T sr = sin(roll * 0.5);

				vector[0] = cr * cp * cy + sr * sp * sy;
				vector[1] = sr * cp * cy - cr * sp * sy;
				vector[2] = cr * sp * cy + sr * cp * sy;
				vector[3] = cr * cp * sy - sr * sp * cy;
			}

			Quaternion operator*(const Quaternion& quat) {
				return Quaternion(
					vector[0] * quat[0] - vector[1] * quat[1] - vector[2] * quat[2] - vector[3] * quat[3],
					vector[0] * quat[1] + vector[1] * quat[0] + vector[2] * quat[3] - vector[3] * quat[2],
					vector[0] * quat[2] - vector[1] * quat[3] + vector[2] * quat[0] + vector[3] * quat[1],
					vector[0] * quat[3] + vector[1] * quat[2] - vector[2] * quat[1] + vector[3] * quat[0]
				);
			}

			Quaternion operator*(const Vector<3, T>& vec) {
				return *this * Quaternion(0, vec[0], vec[1], vec[2]);
			}

			Quaternion operator*(T scalar) {
				return Quaternion(
					vector[0] * scalar,
					vector[1] * scalar,
					vector[2] * scalar,
					vector[3] * scalar
				);
			}

			Quaternion operator/(T scalar) {
				return Quaternion(
					vector[0] / scalar,
					vector[1] / scalar,
					vector[2] / scalar,
					vector[3] / scalar
				);
			}

			T magnitude() {
				return sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2] + vector[3] * vector[3]);
			}

			Quaternion conjugate() {
				return Quaternion(vector[0], -vector[1], -vector[2], -vector[3]);
			}

			Quaternion inverse() {
				return conjugate() / magnitude();
			}

			Vector<3, T> imaginary() {
				return Vector<3, T>({ vector[1], vector[2], vector[3] });
			}

			T real() {
				return vector[0];
			}

			Vector<3, T> rotatePoint(Vector<3, T> point) {
				Quaternion rot = (*this * point) * conjugate();
				return rot.imaginary();
			}

			Vector<3, T> eulerAngles() {
				T a = vector[0];
				T b = vector[1];
				T c = vector[2];
				T d = vector[3];
				T yaw = atan((2 * (a * b + c * d)) / (a * a - b * b - c * c + d * d));
				T pitch = asin(2 * (b * d - a * c));
				T roll = atan((2 * (a * d + b * c)) / (a * a + b * b - c * c - d * d));
				return Vector<3, T>({ yaw, pitch, roll });
			}

		};

		template<class T>
		Quaternion<T> operator *(const Quaternion<T>& lhs, const Quaternion<T>& rhs) {
			return Quaternion({
				lhs[0] * rhs[0] - lhs[1] * rhs[1] - lhs[2] * rhs[2] - lhs[3] * rhs[3],
				lhs[0] * rhs[1] + lhs[1] * rhs[0] + lhs[2] * rhs[3] - lhs[3] * rhs[2],
				lhs[0] * rhs[2] - lhs[1] * rhs[3] + lhs[2] * rhs[0] + lhs[3] * rhs[1],
				lhs[0] * rhs[3] + lhs[1] * rhs[2] + lhs[2] * rhs[1] + lhs[3] * rhs[0]
			});
		}

		template<class T>
		Quaternion<T> operator *(T scalar, const Quaternion<T>& rhs) {
			return Quaternion({
				rhs[0] * scalar,
				rhs[1] * scalar,
				rhs[2] * scalar,
				rhs[3] * scalar
			});
		}

		template<class T>
		Quaternion<T> operator*(Vector<3, T> vec, const Quaternion<T>& rhs) {
			return Quaternion(0, vec[0], vec[1], vec[2]) * rhs;
		}
	}
}

#endif