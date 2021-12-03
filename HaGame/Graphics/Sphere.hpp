#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Mesh.h"
#include "Triangle.hpp"

namespace hagame {
	namespace graphics {
		class SphereMesh {
			Ptr<Mesh> m_mesh;
			float m_radius;
			int m_parallels;
			int m_meridians;

			void computeMesh() {
				Array<Vec3> points;
				Array<Vertex> vertices;
				Array<unsigned int> indices;

				for (int j = 0; j < m_parallels - 1; ++j) {
					float polar = PI * (double)(j + 1) / (double)m_parallels;
					float sp = sin(polar);
					float cp = cos(polar);

					for (int i = 0; i < m_meridians; ++i) {
						float azimuth = 2.0 * PI * (double)i / (double)m_meridians;
						float sa = sin(azimuth);
						float ca = cos(azimuth);
						
						points.push_back(Vec3({ sp * ca, cp, sp * sa }));
					}
				}

				for (int i = 0; i < m_meridians; ++i) {
					auto a = i + 1;
					auto b = (i + 1) % m_meridians + 1;
					Triangle(points[0], points[b], points[a]).insert(vertices, indices);
				}

				for (int j = 0; j < m_parallels - 2; ++j) {
					auto aStart = j * m_meridians + 1;
					auto bStart = (j + 1) * m_meridians + 1;
					for (int i = 0; i < m_meridians; ++i) {
						auto a = aStart + i;
						auto a1 = aStart + (i + 1) % m_meridians;
						auto b = bStart + i;
						auto b2 = bStart + (i + 1) % m_meridians;

						Triangle(a, a1, b).insert(vertices, indices);
						Triangle(a, b, b2).insert(vertices, indices);
					}
				}

				for (int i = 0; i < m_meridians; ++i) {
					auto a = i + m_meridians * (m_parallels - 2) + 1;
					auto b = (i + 1) % m_meridians + m_meridians * (m_parallels - 2) + 1;
					Triangle(points[points.size() - 1], points[a], points[b]).insert(vertices, indices);
				}

				m_mesh->update(vertices, indices);
			}

		public:

			SphereMesh(float radius, int parallels = 10, int meridians = 10) :
				m_radius(radius),
				m_parallels(parallels),
				m_meridians(meridians) {
				m_mesh = std::make_shared<Mesh>();
				computeMesh();
			}

			void setRadius(float radius) { m_radius = radius; computeMesh(); }
			float getRadius() { return m_radius; }

			void setParallels(int parallels) { m_parallels = parallels; computeMesh(); }
			int getParallels() { return m_parallels; }

			void setMeridians(int meridians) { m_meridians = meridians; computeMesh(); }
			int getMeridians() { return m_meridians; }

			Mesh* getMesh() { return m_mesh.get(); }
		};
	}
}

#endif
