#ifndef PLANE_H
#define PLANE_H

#include "DynamicMesh.hpp"

namespace hagame {
	namespace graphics {

		template <size_t M, size_t N>
		class Plane : public DynamicMesh {

			Ptr<Mesh> m_mesh;
			Vec2Int m_divisions;
			Array<Array<Vec3>> m_points;
			Vec2 m_cellSize;

		public:

			// Wrapper around mesh that is produces a plane of a given size divided into triangles. Built for terrain generation
			Plane(Vec2 size) : 
				m_cellSize(size.div(Vec2({ (float)M, (float)N }))),
				m_divisions(Vec2Int({ M, N })),
				m_mesh(std::make_shared<Mesh>())
			{
				for (int i = 0; i < M; i++) {
					Array<Vec3> row;
					for (int j = 0; j < N; j++) {
						row.push_back(Vec3({ i * m_cellSize[0], 0.0f, j * m_cellSize[1] }));
					}
					m_points.push_back(row);
				}
				computeMesh();
			}

			void computeMesh() {
				/*
					D--------C
					-        -
					-        -
					-        -
					A--------B

					Generate triangles: ABD & BCD for each cell in the grid
				*/
				Array<Vertex> vertices;
				Array<unsigned int> indices;

				for (int i = 0; i < M - 1; i++) {
					for (int j = 0; j < N - 1; j++) {

						Vec3 a, b, c, d;
						a = m_points[i][j];
						b = m_points[i][j + 1];
						c = m_points[i + 1][j + 1];
						d = m_points[i + 1][j];

						Triangle A = Triangle(a, b, c);
						Triangle B = Triangle(a, c, d);

						A.insert(vertices, indices);
						B.insert(vertices, indices);
					}
				}

				m_mesh->update(vertices, indices);
			}

			Vec2 getCellSize() { return m_cellSize; }

			Vec2Int getDivisions() { return m_divisions; }

			void setDivisions(Vec2Int divisions) { if (divisions != m_divisions) { m_divisions = disivisions; computeMesh(); } }

			void setSize(Vec2 size) { 
				auto tmpSize = size.div(Vec2({ (float)M, (float)N }));
				if (tmpSize != m_cellSize) {
					m_cellSize = tmpSize;
					computeMesh();
				}
			}

			void setPoint(Vec2Int idx, Vec3 point) {
				assert(idx[0] >= 0 && idx[0] < M && idx[1] >= 0 && idx[1] < N);
				m_points[idx[0]][idx[1]] = point;
			}

			Vec3 getPoint(Vec2Int idx) {
				assert(idx[0] >= 0 && idx[0] < M && idx[1] >= 0 && idx[1] < N);
				return m_points[idx[0]][idx[1]];
			}

			Mesh* getMesh() { return m_mesh.get(); }
			
		};
	}
}

#endif
