#ifndef VECTOR
#define VECTOR

#include <functional>
#include <string>
#include <vector>
#include <cmath>

namespace hagame {
	namespace math {
		
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

			// Basic accessors
			const T x() {
				return vector[0];
			}

			const T y() {
				return vector[1];
			}

			const T z() {
				return vector[2];
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

			T magnitude() {
				T sum = 0;
				for (int i = 0; i < size; i++) { sum += vector[i] * vector[i]; }
				return sqrt(sum);
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

			T dot(Vector vect) {
				T sum = 0;
				for (int i = 0; i < size; i++) {
					sum += vector[i] * vect[i];
				}
				return sum;
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
	}
}

#endif