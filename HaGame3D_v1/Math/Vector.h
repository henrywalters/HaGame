#include <functional>
#include <string>
#include <vector>

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

			Vector(const Vector& vect) {
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

			// Basic functions

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
				T magnitude = copy.magnitude();
				for (int i = 0; i < size; i++) {
					copy[i] /= magnitude;
				}
				return copy;
			}

			void normalize() {
				T magnitude = magnitude();
				for (int i = 0; i < size; i++) {
					vector[i] /= magnitude;
				}
			}

			T dot(Vector vect) {
				T sum = 0;
				for (int i = 0; i < size; i++) {
					sum += vector[i] * vect[i];
				}
				return sum;
			}

			Vector prod(Vector vect) {
				Vector copy = *this;
				for (int i = 0; i < size; i++) {
					copy[i] *= vect[i];
				};
				return copy;
			}

			// Operator Overloads

			T operator[](int i) const {
				return vector[i];
			}

			T& operator[](int i) {
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