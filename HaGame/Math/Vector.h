#ifndef VECTOR
#define VECTOR

#include <functional>
#include <string>
#include <vector>
#include <cmath>
#include "./../Core/Object.h"
#include "./../Vendor/json.hpp"

using JSON = nlohmann::json;

namespace hagame {
	namespace math {
		
		template <class T>
		class Quaternion;

		// Highly generic vector class that can be extended to fit any needs.
		template<size_t size, class T>
		class Vector {
		public:

			T vector[size];

			Vector(std::vector<T> vect) {
				for (int i = 0; i < size; i++) {
					vector[i] = i < vect.size() ? vect[i] : 0;
				}
			}

			Vector() {
				for (int i = 0; i < size; i++) {
					vector[i] = 0;
				}
			}
			
			Vector(T val) {
				for (int i = 0; i < size; i++) {
					vector[i] = val;
				}
			}

			Vector(const Vector& vect) {
				for (int i = 0; i < size; i++) {
					vector[i] = vect[i];
				}
			}

			Vector(T vect[size]) {
				for (int i = 0; i < size; i++) {
					vector[i] = vect[i];
				}
			}

			// Static Helpers

			static Vector Identity() {
				Vector vect;
				for (int i = 0; i < size; i++) {
					vect[i] = 1;
				}
				return vect;
			}

			static Vector Zero() {
				Vector vect;
				return vect;
			}

			static Vector<3, T> Right() {
				return Vector({ 1, 0, 0 });
			}

			static Vector<3, T> Left() {
				return Vector({ -1, 0, 0 });
			}

			static Vector<3, T> Top() {
				return Vector({ 0, 1, 0 });
			}

			static Vector<3, T> Bottom() {
				return Vector({ 0, -1, 0 });
			}

			static Vector<3, T> Face() {
				return Vector({ 0, 0, 1 });
			}

			static Vector<3, T> Back() {
				return Vector({ 0, 0, -1 });
			}

			static Vector<3, T> Right(T value) {
				return Vector({ value, 0, 0 });
			}

			static Vector<3, T> Left(T value) {
				return Vector({ -value, 0, 0 });
			}

			static Vector<3, T> Top(T value) {
				return Vector({ 0, value, 0 });
			}

			static Vector<3, T> Bottom(T value) {
				return Vector({ 0, -value, 0 });
			}

			static Vector<3, T> Face(T value) {
				return Vector({ 0, 0, value });
			}

			static Vector<3, T> Back(T value) {
				return Vector({ 0, 0, -value });
			}

			// Basic accessors
			constexpr T x() {
				assert(size >= 1);
				return vector[0];
			}

			constexpr T y() {
				assert(size >= 2);
				return vector[1];
			}

			constexpr T z() {
				assert(size >= 3);
				return vector[2];
			}

			constexpr Vector<2, T> xy() {
				assert(size >= 3);
				return Vector<2, T>({ vector[0], vector[1] });
			}

			constexpr Vector<2, T> xz() {
				assert(size >= 3);
				return Vector<2, T>({ vector[0], vector[2] });
			}

			constexpr Vector<2, T> yz() {
				assert(size >= 3);
				return Vector<2, T>({ vector[1], vector[2] });
			}

			// Basic functions

			// Set all elements to zero

			Vector copy() {
				Vector copy = Vector();
				for (int i = 0; i < size; i++) {
					copy[i] = vector[i];
				}
				return copy;
			}

			void zero() {
				for (int i = 0; i < size; i++) {
					vector[i] = 0;
				}
			}

			// Update all values of vector based on index and value
			void update(std::function<T(int, T)> lambda) {
				for (int i = 0; i < size; i++) {
					vector[i] = lambda(i, vector[i]);
				}
			}

			size_t getSize() {
				return size;
			}

			std::string toString() {
				std::string out = "[";
				for (int i = 0; i < size; i++) {
					if (i > 0) out += ", ";
					out += std::to_string(vector[i]);
				}
				out += "]";
				return out;
			}

			const T magnitude() {
				T sum = 0;
				for (int i = 0; i < size; i++) { sum += vector[i] * vector[i]; }
				return sqrt(sum);
			}

			const T magnitudeSq() {
				T mag = magnitude();
				return mag * mag;
			}

			Vector normalized() {
				Vector copy = *this;
				T mag = copy.magnitude();
				if (mag != 0) {
					for (int i = 0; i < size; i++) {
						copy[i] /= mag;
					}
				}
				return copy;
			}

			void normalize() {
				T mag = magnitude();
				if (mag != 0) {
					for (int i = 0; i < size; i++) {
						vector[i] /= mag;
					}
				}
				
			}

			Vector rounded(float step = 1.0f) {
				Vector copy = *this;
				for (int i = 0; i < size; i++) { 
					copy[i] = std::round(copy[i]); 
					//copy[i] = (float)std::floor(copy[i] * step + 0.5f) / step;
				}
				return copy;
			}

			void round() {
				for (int i = 0; i < size; i++) { vector[i] = std::round(vector[i]); }
			}

			Vector floor() {
				Vector copy = *this;
				for (int i = 0; i < size; i++) { copy[i] = std::floor(copy[i]); }
				return copy;
			}

			Vector ceil() {
				Vector copy = *this;
				for (int i = 0; i < size; i++) { copy[i] = std::ceil(copy[i]); }
				return copy;
			}

			T dot(Vector vect) {
				T sum = 0;
				for (int i = 0; i < size; i++) {
					sum += vector[i] * vect[i];
				}
				return sum;
			}

			T angleBetween(Vector vect) {
				return atan2(cross(vect).dot(Vec3::Face()), dot(vect));
			}

			Quaternion<T> rotationBetween(Vector vect) {
				float dotProd = dot(vect);
				float k = sqrt(magnitudeSq() * vect.magnitudeSq());

				if (dotProd / k == -1) {
					// TODO: fix this
					return Quaternion<T>(0, orth().normalized());
				}

				Quaternion q = Quaternion(0.5f * PI * (dotProd + k), cross(vect));
				q.normalize();

				return q;
			}

			// Compute an orthaganol vector
			Vector orth() {
				if (*this != Vector::Top()) {
					return cross(Vector::Top());
				}
				else if (*this != Vector::Right()) {
					return cross(Vector::Right());
				}
				else {
					return cross(Vector::Face());
				}
			}

			// Return the vector such that V.prod(V.inverse()) = V.Identity();
			Vector inverse() {
				Vector copy = *this;
				for (int i = 0; i < size; i++) {
					copy[i] = 1 / vect[i];
				};
				return copy;
			}


			Vector<3, T> cross(Vector vect) {
				if (size != 3) {
					throw new std::exception("Cross product only supported for 3d vectors");
				}

				return Vector<3, T>({
					vector[1] * vect[2] - vector[2] * vect[1],
					-(vector[0] * vect[2] - vector[2] * vect[0]),
					vector[0] * vect[1] - vector[1] * vect[0]
				});
			}

			// element-wise multiplication
			Vector prod(Vector vect) {
				Vector copy = *this;
				for (int i = 0; i < size; i++) {
					copy[i] *= vect[i];
				};
				return copy;
			}

			// element-wise division
			Vector div(Vector vect) {
				Vector copy = *this;
				for (int i = 0; i < size; i++) {
					copy[i] /= vect[i];
				};
				return copy;
			}

			// Return vect projected onto this vector
			Vector proj(Vector vect) {
				return vect * (dot(vect) / vect.dot(vect));
			}

			// Cast vector from type T to V
			template <class V>
			Vector<size, V> cast() {
				Vector<size, V> copy = Vector<size, V>();
				for (int i = 0; i < size; i++) {
					copy[i] = static_cast<V>(vector[i]);
				}
				return copy;
			}

			template <size_t toSize>
			Vector<toSize, T> resize(T defaultValue = 0.0) {
				Vector<toSize, T> copy = Vector<toSize, T>();
				for (int i = 0; i < toSize; i++) {
					if (i < size) {
						copy.vector[i] = vector[i];
					}
					else {
						copy.vector[i] = defaultValue;
					}
				}
				return copy;
			}

			T min() {
				T val;
				bool first = true;
				for (int i = 0; i < size; i++) {
					if (first) {
						val = vector[i];
						first = false;
					}
					else if (vector[i] < val)
						val = vector[i];
				}
				return val;
			}

			T max() {
				T val;
				bool first = true;
				for (int i = 0; i < size; i++) {
					if (first) {
						val = vector[i];
						first = false;
					}
					else if (vector[i] > val)
						val = vector[i];
				}
				return val;
			}

			Vector bounded(Vector by) {
				auto out = copy();
				float maxDim = max();
				float byMaxDim = by.max();
				float scale = maxDim / byMaxDim;

				if (scale > 1) {
					out *= 1 / scale;
				}

				return out;
			}

			Vector fill(Vector to) {
				auto out = copy();
				float scale = max() / to.max();
				return out * (1 / scale);
			}

			// Operator Overloads

			T operator[](int i) const {
				if (i >= size) {
					throw new std::exception("Index out of bounds");
				}
				return vector[i];
			}

			T& operator[](int i) {
				if (i >= size) {
					throw new std::exception("Index out of bounds");
				}
				return vector[i];
			}

			bool operator==(const Vector& vect) {
				for (int i = 0; i < size; i++) {
					if (vector[i] != vect[i]) return false;
				}
				return true;
			}

			bool operator!=(const Vector& vect) {
				for (int i = 0; i < size; i++) {
					if (vector[i] != vect[i]) return true;
				}
				return false;
			}

			bool operator<(const Vector& vect) const noexcept {
				T magA, magB = 0.0f;
				for (int i = 0; i < size; i++) {
					magA += vector[i] * vector[i];
					magB += vect[i] * vect[i];
				}
				magA = sqrt(magA);
				magB = sqrt(magB);

				return magA < magB;
			}

			Vector operator+(const Vector& vect) {
				Vector copy = *this;
				for (int i = 0; i < size; i++) {
					copy[i] += vect[i];
				}
				return copy;
			}

			Vector operator-(const Vector& vect) {
				Vector copy = *this;
				for (int i = 0; i < size; i++) {
					copy[i] -= vect[i];
				}
				return copy;
			}

			Vector operator*(T scalar) {
				Vector copy = *this;
				for (int i = 0; i < size; i++) {
					copy[i] *= scalar;
				}
				return copy;
			}

			Vector operator/(T scalar) {
				Vector copy = *this;
				for (int i = 0; i < size; i++) {
					copy[i] /= scalar;
				}
				return copy;
			}

			Vector& operator+=(const Vector& vect) {
				for (int i = 0; i < size; i++) {
					vector[i] += vect[i];
				}
				return *this;
			}

			Vector& operator-=(const Vector& vect) {
				for (int i = 0; i < size; i++) {
					vector[i] -= vect[i];
				}
				return *this;
			}

			Vector& operator*=(T scalar) {
				for (int i = 0; i < size; i++) {
					vector[i] *= scalar;
				}
				return *this;
			}

			Vector& operator/=(T scalar) {
				for (int i = 0; i < size; i++) {
					vector[i] /= scalar;
				}
				return *this;
			}
		};

		template<size_t size, class T>
		Vector<size, T> operator *(T scalar, const Vector<size, T>& rhs) {
			Vector<size, T> copy = Vector<size, T>(rhs);
			return copy * scalar;
		}

		template<size_t size, class T>
		void to_json(JSON& json, const Vector<size, T>& vect) {
			json = vect.vector;
		}

		template<size_t size, class T>
		void from_json(const JSON& json, Vector<size, T>& vect) {
			for (int i = 0; i < size; i++) {
				json.at(i).get_to(vect[i]);
			}
		}

	}
}

#endif