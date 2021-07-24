#ifndef PLANE_H
#define PLANE_H

#include "Mesh.h"

namespace hagame {
	namespace graphics {

		template <size_t M, size_t N>
		class Plane {

		public:

			Array<Array<Vec3>> points;
			Vec2 cellSize;

			const Vec2Int divisions;

			// Wrapper around mesh that is produces a plane of a given size divided into triangles. Built for terrain generation
			Plane(Vec2 _size) : divisions(Vec2Int({ M, N })) {
				cellSize = _size.div(Vec2({ (float) M, (float) N }));
				for (int i = 0; i < M; i++) {
					Array<Vec3> row;
					for (int j = 0; j < N; j++) {
						row.push_back(Vec3({ i * cellSize[0], 0.0f, j * cellSize[1] }));
					}
					points.push_back(row);
				}
			}

			Ptr<Mesh> generateMesh() {
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
				unsigned int idx = 0;

				for (int i = 0; i < M - 1; i++) {
					for (int j = 0; j < N - 1; j++) {
						Vec3 a, b, c, d;
						Vertex v1, v2, v3, v4;

						a = points[i][j];
						b = points[i + 1][j];
						c = points[i + 1][j + 1];
						d = points[i][j + 1];

						v1.position = a;
						v2.position = b;
						v3.position = c;
						v4.position = d;

						Vec3 normal = (d - a).dot(b - a);

						v1.normal = normal;
						v2.normal = normal;
						v3.normal = normal;
						v4.normal = normal;

						v1.texCoords = Vec2({ 0, 0 });
						v2.texCoords = Vec2({ 1, 0 });
						v3.texCoords = Vec2({ 1, 1 });
						v4.texCoords = Vec2({ 0, 1 });

						vertices.insert(vertices.end(), { v1, v4, v2, v2, v4, v3 });
						indices.insert(indices.end(), { idx, idx + 1, idx + 2, idx + 3, idx + 4, idx + 5 });

						idx += 6;
					}
				}

				return std::make_shared<Mesh>(vertices, indices);
			}
		};
	}
}

#endif
