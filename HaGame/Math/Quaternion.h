#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector.h"

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
				*this[0] = cos(0.5 * rotation);
				*this[1] = axisOfRotation[0] * sin(0.5 * rotation);
				*this[2] = axisOfRotation[1] * sin(0.5 * rotation);
				*this[3] = axisOfRotation[2] * sin(0.5 * rotation);
			}

			Quaternion(T w, T i, T j, T k) {
				*this[0] = w;
				*this[1] = i;
				*this[2] = j;
				*this[3] = k;
			}

			Quaternion(Vector<3, T> euler) : Quaternion(euler[0], euler[1], euler[2]) {}

			Quaternion(T roll, T pitch, T yaw) {
				T cy = cos(yaw * 0.5);
				T sy = sin(yaw * 0.5);
				T cp = cos(pitch * 0.5);
				T sp = sin(pitch * 0.5);
				T cr = cos(roll * 0.5);
				T sr = sin(roll * 0.5);

				*this[0] = cr * cp * cy + sr * sp * sy;
				*this[1] = sr * cp * cy - cr * sp * sy;
				*this[2] = cr * sp * cy + sr * cp * sy;
				*this[3] = cr * cp * sy - sr * sp * cy;
			}

			Quaternion operator*(const Quaternion& quat) {
				return Quaternion(
					*this[0] * quat[0] - *this[1] * quat[1] - *this[2] * quat[2] - *this[3] * quat[3],
					*this[0] * quat[1] + *this[1] * quat[0] + *this[2] * quat[3] - *this[3] * quat[2],
					*this[0] * quat[2] - *this[1] * quat[3] + *this[2] * quat[0] + *this[3] * quat[1],
					*this[0] * quat[3] + *this[1] * quat[2] - *this[2] * quat[1] + *this[3] * quat[0]
				);
			}

			Quaternion operator*(const Vector<3, T>& vec) {
				return *this * Quaternion(0, vec[0], vec[1], vec[2]);
			}

			Quaternion operator*(T scalar) {
				return Quaternion(
					*this[0] * scalar,
					*this[1] * scalar,
					*this[2] * scalar,
					*this[3] * scalar
				);
			}

			Quaternion operator/(T scalar) {
				return Quaternion(
					*this[0] / scalar,
					*this[1] / scalar,
					*this[2] / scalar,
					*this[3] / scalar
				);
			}

			T magnitude() {
				return sqrt(*this[0] * *this[0] + *this[1] * *this[1] + *this[2] * *this[2] + *this[3] * *this[3]);
			}

			Quaternion conjugate() {
				return Quaternion(*this[0], -*this[1], -*this[2], -*this[3]);
			}

			Quaternion inverse() {
				return conjugate() / magnitude();
			}

			Vector<3, T> imaginary() {
				return Vector<3, T>({ *this[1], *this[2], *this[3] });
			}

			T real() {
				return *this[0];
			}

			Vector<3, T> rotatePoint(Vector<3, T> point) {
				return ((*this * point) * conjugate()).imaginary();
			}

			Vector<3, T> eulerAngles() {
				T a = *this[0];
				T b = *this[1];
				T c = *this[2];
				T d = *this[3];

				// Roll
				T roll_a = 2 * (a * b + c * d);
				T roll_b = 1 - 2 * (b * b + c + c);
				T roll = atan2(roll_a, roll_b);

				// Pitch
				T pitch = 2 * (a * c - d * b);
				if (abs(pitch) >= 1)
					pitch = std::copysign(M_PI / 2, pitch);
				else
					pitch = asin(pitch);

				// Yaw
				T yaw_a = 2 * (a * d + b * c);
				T yaw_b = 1 - 2 * (c * c + d * d);
				T yaw = atan2(yaw_a, yaw_b);

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