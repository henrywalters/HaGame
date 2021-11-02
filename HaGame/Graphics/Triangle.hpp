#ifndef G_TRIANGLE_HPP
#define G_TRIANGLE_HPP

#include "./Mesh.h"
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		struct Triangle {

			Array<Vertex> verts;

			Triangle(Vec3 a, Vec3 b, Vec3 c) {
				for (int i = 0; i < 3; i++) {
					verts.push_back(Vertex());
				}
				setPositions({ a, b, c });
				setTextures({ a.resize<2>(), b.resize<2>(), c.resize<2>() });
				setNormal(cross(b - a, c - a));
			}

			Triangle(math::Triangle tri): Triangle(tri.a, tri.b, tri.c) {}

			void setPositions(Array<Vec3> points) {
				for (int i = 0; i < 3; i++) {
					verts[i].position = points[i];
				}
			}

			void setNormal(Vec3 normal) {
				setNormals({ normal, normal, normal });
			}

			void setNormals(Array<Vec3> normals) {
				for (int i = 0; i < 3; i++) {
					verts[i].normal = normals[i];
				}
			}

			void setTextures(Array<Vec2> textures) {
				for (int i = 0; i < 3; i++) {
					verts[i].texCoords = textures[i];
				}
			}

			Ptr<Mesh> getMesh() {

				Array<unsigned int> indices = { 0, 1, 2 };

				return std::make_shared<Mesh>(
					verts,
					indices
				);
			}

			// Insert the contents of this triangle into the data structures needed to construct a Mesh
			void insert(Array<Vertex>& vertices, Array<unsigned int>& indices) {
				unsigned int lastIdx = indices.size();
				indices.insert(indices.end(), { lastIdx, lastIdx + 1, lastIdx + 2 });
				vertices.insert(vertices.end(), { verts[0], verts[1], verts[2] });
			}
		};
	}
}

#endif
