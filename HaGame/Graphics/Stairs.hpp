#ifndef STAIRS_HPP
#define STAIRS_HPP

#include "Mesh.h"
#include "Triangle.hpp"
#include "DynamicMesh.hpp"
#include "./../Vendor/imgui/imgui.h"

namespace hagame {
	namespace graphics {
		class Stairs : public DynamicMesh {

			unsigned int m_steps;
			Vec3 m_size;
			Ptr<Mesh> m_mesh;

			void computeMesh() {
				Array<Vertex> vertices;
				Array<unsigned int> indices;

				float height = m_size[1] / (float)m_steps;
				float depth = m_size[2] / (float)m_steps;
				float radius = m_size[0] / 2.0f;

				for (int x = 0; x < m_steps; x++) {

					float h1 = -m_size[1] / 2.0f;
					float h2 = h1 + x * height;
					float h3 = h1 + (x + 1) * height;

					Vec3 a = Vec3(-radius, h2, x * depth);
					Vec3 b = Vec3(-radius, h3, x * depth);
					Vec3 c = Vec3(radius, h3, x * depth);
					Vec3 d = Vec3(radius, h2, x * depth);
					Vec3 e = Vec3(-radius, h3, (x + 1) * depth);
					Vec3 f = Vec3(radius, h3, (x + 1) * depth);
					Vec3 g = Vec3(-radius, h1, x * depth);
					Vec3 h = Vec3(radius, h1, x * depth);
					Vec3 i = Vec3(-radius, h1, (x + 1) * depth);
					Vec3 j = Vec3(radius, h1, (x + 1) * depth);

					Triangle A = Triangle(a, b, c);
					Triangle B = Triangle(a, c, d);
					Triangle C = Triangle(b, e, f);
					Triangle D = Triangle(b, f, c);

					C.setTextures({ a.resize<2>(), b.resize<2>(), c.resize<2>() });
					D.setTextures({ a.resize<2>(), c.resize<2>(), d.resize<2>() });

					A.insert(vertices, indices);
					B.insert(vertices, indices);
					C.insert(vertices, indices);
					D.insert(vertices, indices);
					
				}

				m_mesh->update(vertices, indices);
			}

		public:

			Stairs(Vec3 size, unsigned int steps) : m_size(size), m_steps(steps) {
				m_mesh = std::make_shared<Mesh>();
				computeMesh();
			}

			void setSteps(unsigned int steps) { if (steps != m_steps) { m_steps = steps; computeMesh(); } }
			unsigned int getSteps() { return m_steps; }

			void setSize(Vec3 size) { if (size != m_size) { m_size = size; computeMesh(); } }
			Vec3 getSize() { return m_size; }

			Mesh* getMesh() { return m_mesh.get(); }
		};
	}
}

#endif
