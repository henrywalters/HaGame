#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include "Quaternion.h"
#include "Hypercube.h"

namespace hagame {
	namespace math {

		template <class T> inline T sq(T val) { return val * val; }

		template <size_t M, size_t N, class T> 
		class Matrix {
		public:
			T mat[M * N];

			Matrix() {
				fill(0);
			}

			Matrix(T value) {
				fill(value);
			}

			Matrix(std::vector<Vector<N, T>> rows) {
				for (int i = 0; i < M; i++) {
					for (int j = 0; j < N; j++) {
						mat[Matrix::FlattenIndex(i, j)] = rows[i][j];
					}
				}
			}

			Matrix(std::vector<T> values) {

				if (values.size() != M * N) {
					throw new std::exception("Invalid data length for this size matrix");
				}

				for (int i = 0; i < M * N; i++) {
					mat[i] = values[i];
				}
			}

			// Static Initializers

			static Matrix Identity() {
				Matrix mat;
				mat.fillDiag(1);
				return mat;
			}

			static Matrix<4, 4, T> Scale(Vector<3, T> scale) {
				std::vector<T> mat = {
					scale.x(), 0, 0, 0,
					0, scale.y(), 0, 0,
					0, 0, scale.z(), 0,
					0, 0, 0, 1,
				};
				return Matrix<4, 4, T>(mat);
			}

			static Matrix<4, 4, T> Translation(Vector<3, T> translation) {
				std::vector<T> trans = {
					1, 0, 0, translation.x(),
					0, 1, 0, translation.y(),
					0, 0, 1, translation.z(),
					0, 0, 0, 1
				};
				return Matrix<4, 4, T>(trans);
			}

			static Matrix<4, 4, T> Rotation(Quaternion<T> quat) {
				std::vector<T> rot = {
					sq(quat[0]) + sq(quat[1]) - sq(quat[2]) - sq(quat[3]), 2 * quat[1] * quat[2] - 2 * quat[0] * quat[3], 2 * quat[1] * quat[3] + 2 * quat[0] * quat[2], 0,
					2 * quat[1] * quat[2] + 2 * quat[0] * quat[3], sq(quat[0]) - sq(quat[1]) + sq(quat[2]) - sq(quat[3]), 2 * quat[2] * quat[3] - 2 * quat[0] * quat[1], 0,
					2 * quat[1] * quat[3] - 2 * quat[0] * quat[2], 2 * quat[2] * quat[3] + 2 * quat[0] * quat[1], sq(quat[0]) - sq(quat[1]) - sq(quat[2]) + sq(quat[3]), 0,
					0, 0, 0, 1
				};
				return Matrix<4, 4, T>(rot);
			}

			static Matrix<4, 4, T> Orthographic(T left, T right, T bottom, T top, T zNear, T zFar) {
				std::vector<T> orth = {
					2 / (right - left), 0, 0, -(right + left) / (right - left),
					0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
					0, 0, -2 / (zFar - zNear), -(zFar + zNear) / (zFar - zNear),
					0, 0, 0, 1
				};
				return Matrix<4, 4, T>(orth);
			}

			static Matrix<4, 4, T> Orthographic(Hypercube<3, T> projSpace) {
				return Matrix<4, 4, T>::Orthographic(
					projSpace.pos[0],
					projSpace.pos[0] + projSpace.size[0],
					projSpace.pos[1],
					projSpace.pos[1] + projSpace.size[1],
					projSpace.pos[2],
					projSpace.pos[2] + projSpace.size[2]
				);
			}

			static Matrix<4, 4, T> Perspective(Hypercube<3, T> projSpace) {
				return Matrix<4, 4, T>::Perspective(
					projSpace.pos[0],
					projSpace.pos[0] + projSpace.size[0],
					projSpace.pos[1],
					projSpace.pos[1] + projSpace.size[1],
					projSpace.pos[2],
					projSpace.pos[2] + projSpace.size[2]
				);
			}

			static Matrix<4, 4, T> Perspective(T fov, T aspect, T zNear, T zFar) {
				T top = tan(fov / 2) * zNear;
				T bottom = -top;
				T right = top * aspect;
				T left = -top * aspect;
				return Perspective(left, right, bottom, top, zNear, zFar);
			}

			static Matrix<4, 4, T> Perspective(T left, T right, T bottom, T top, T zNear, T zFar) {
				
				std::vector<T> proj = {
					2 * zNear / (right - left), 0, (right + left) / (right - left), 0,
					0, 2 * zNear / (top - bottom), (top + bottom) / (top - bottom), 0,
					0, 0, -(zFar + zNear) / (zFar - zNear), -2 * zNear * zFar / (zFar - zNear),
					0, 0, -1, 0
				};

				return Matrix<4, 4, T>(proj);
			}

			static Matrix<4, 4, T> LookAt(Vector<3, T> eye, Vector<3, T> target, Vector<3, T> up) {
				Vector<3, T> zAxis = (eye - target).normalized();
				Vector<3, T> xAxis = (up.cross(zAxis)).normalized();
				Vector<3, T> yAxis = zAxis.cross(xAxis);

				std::vector<T> view = {
					xAxis[0], xAxis[1], xAxis[2], -xAxis.dot(eye),
					yAxis[0], yAxis[1], yAxis[2], -yAxis.dot(eye),
					zAxis[0], zAxis[1], zAxis[2], -zAxis.dot(eye),
					0, 0, 0, 1
				};
				
				return Matrix<4, 4, T>(view);
			}

			static Matrix<4, 4, T> FPSView(Vector<3, T> eye, float pitch, float yaw) {
				float cosPitch = cos(pitch);
				float sinPitch = sin(pitch);
				float cosYaw = cos(yaw);
				float sinYaw = sin(yaw);

				Vector<3, T> xAxis = Vector<3, T>({ cosYaw, 0, -sinYaw });
				Vector<3, T> yAxis = Vector<3, T>({ sinYaw * sinPitch, cosPitch, cosYaw * sinPitch });
				Vector<3, T> zAxis = Vector<3, T>({ sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw });

				std::vector<T> view = {
					xAxis[0], xAxis[1], xAxis[2], -xAxis.dot(eye),
					yAxis[0], yAxis[1], yAxis[2], -yAxis.dot(eye),
					zAxis[0], zAxis[1], zAxis[2], -zAxis.dot(eye),
					0, 0, 0, 1
				};

				return Matrix<4, 4, T>(view);

			}

			// Class methods

			Matrix<M, N, T> copy() {
				Matrix<M, N, T> c = Matrix<M, N, T>();
				for (int i = 0; i < M; i++) {
					for (int j = 0; j < N; j++) {
						c.set(i, j, get(i, j));
					}
				}
				return c;
			}

			const static int rows() {
				return N;
			}

			const static int cols() {
				return M;
			}

			const T get(int row, int col) {
				return mat[Matrix::FlattenIndex(row, col)];
			}

			const void set(int row, int col, T value) {
				mat[Matrix::FlattenIndex(row, col)] = value;
			}

			bool isSquare() {
				return M == N;
			}

			void fill(T value) {
				for (int i = 0; i < M * N; i++) {
					mat[i] = value;
				}
			}

			void fillDiag(T value) {
				if (!isSquare()) {
					throw new std::exception("Can only fill diag of square matrices");
				}

				for (int i = 0; i < M; i++) {
					mat[Matrix::FlattenIndex(i, i)] = value;
				}
			}

			bool reduce() {
				float pro = 0;
				int c = 0;

				int i, j, k = 0;

				for (i = 0; i < N; i++) {
					if (get(i, i) == 0) {
						c = 1;
						while ((c + 1) < N && get(i + c, i) == 0) {
							c++;
						}

						if ((i + c) == N) {
							return false;
						}

						for (j = i, k = 0; k <= N; k++) {
							T tmp = get(j, k);
							set(j, k, get(j + c, k));
							set(j + c, k, tmp);
						}
					}

					for (j = 0; j < N; j++) {
						if (i != j) {
							float pro = get(j, i) / get(i, i);

							for (k = 0; k <= N; k++) {
								set(j, k, get(j, k) - get(i, k) * pro);
							}
						}
					}
				}
			}

			std::string toString() {
				std::string out = "";
				for (int i = 0; i < M; i++) {
					std::string row = "[";
					for (int j = 0; j < N; j++) {
						if (j > 0) row += ", ";
						row += std::to_string(mat[Matrix::FlattenIndex(i, j)]);
					}
					row += "]\n";
					out += row;
				}

				return out;
			}

			// Static helpers
			static int FlattenIndex(int row, int col) {
				return row * N + col;
			}

			const static bool IsSameSize(const Matrix& A, const Matrix& B) {
				return A.cols() == B.cols() && A.rows() == B.rows();
			}

			const static bool CanMultiply(const Matrix& A, const Matrix& B) {
				return A.cols() == B.rows();
			}

			static Vector<2, int> UnpackIndex(int flatIndex) {
				Vector<2, int> idx = Vector<2, int>();
				idx[1] = flatIndex % N;
				idx[0] = (flatIndex - idx[1]) / N;
				return idx;
			}

			// Operators
			T operator[](int i) const {
				return mat[i];
			}

			T& operator[](int i) {
				return mat[i];
			}

			Matrix operator+(const Matrix& B) {
				if (!Matrix::IsSameSize(this, B)) {
					throw new std::exception("Only same size matrices may be added");
				}

				Matrix out = Matrix();
				for (int i = 0; i < M * N; i++) {
					out[i] = this[i] + B[i];
				}
				return out;
			}

			Matrix operator+=(const Matrix& B) {
				if (!Matrix::IsSameSize(this, B)) {
					throw new std::exception("Only same size matrices may be added");
				}
				for (int i = 0; i < M * N; i++) {
					this[i] += B[i];
				}
			}

			template <size_t P, size_t Q>
			Matrix operator*(const Matrix<P, Q, T> & B) {
				if (!Matrix::CanMultiply(*this, B)) {
					throw new std::exception("Number of cols in mat A must match the number of rows in mat B");
				}

				Matrix<M, Q, T> out = Matrix<M, Q, T>();
				for (int i = 0; i < M; i++) {
					for (int j = 0; j < B.cols(); j++) {
						for (int k = 0; k < B.rows(); k++) {
							out.set(i, j, out.get(i, j) + (get(i, k) * B[Matrix::FlattenIndex(k, j)]));
						}
					}
				}
				return out;
			}
		};

		template <size_t N, size_t M, size_t, size_t P, size_t Q, class T>
		Matrix<M, Q, T> operator * (const Matrix<N, M, T>& A, const Matrix<P, Q, T>& B) {
			Matrix<M, Q, T> out = Matrix<M, Q, T>();
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < B.cols(); j++) {
					for (int k = 0; k < B.rows(); k++) {
						out[i * N + j] += A[i * N + k] * B[k * P + j];
					}
				}
			}
			return out;
		}

		template <size_t N, size_t P, size_t Q, class T>
		Vector<N, T> operator * (const Vector<N, T>& vec, const Matrix<P, Q, T>& mat) {
			Vector<Q, T> out = Vector<Q, T>();
			for (int i = 0; i < Q; i++) {
				for (int j = 0; j < N; j++) {
					out[i] += vec[j] * mat[j * P + i];
				}
			}
			return out;
		}
	}
}

#endif