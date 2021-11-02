#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Mesh.h"
#include "Triangle.hpp"
#include <cstdint>

namespace hagame {
	namespace graphics {
		class Cylinder {
			
			Ptr<Mesh> m_mesh;
			int m_divisions;
			float m_radius;
			float m_height;

			void computeMesh() {
				Array<Vertex> vertices;
				Array<unsigned int> indices;
				unsigned int idx = 0;

				float theta = (2 * PI) / m_divisions;
				float width = (1.0f / m_divisions) * 2 * PI * m_radius;

				Vec3 top = Vec3({ 0.0f, m_height / 2.0f, 0.0f });
				Vec3 bottom = Vec3({ 0.0f, -m_height / 2.0f, 0.0f });
				
				for (int i = 0; i < m_divisions; i++) {
					// Add two triangles for top and bottom of cylinder and the face between them

					Vec3 centerA = Vec3({ cos(i * theta), 0.0f, sin(i * theta) });
					Vec3 centerB = Vec3({ cos((i + 1) * theta), 0.0f, sin((i + 1) * theta) });

					Vec3 a = centerA + bottom;
					Vec3 b = centerA + top;
					Vec3 c = centerB + top;
					Vec3 d = centerB + bottom;

					Triangle triA = Triangle(a, b, c);
					Triangle triB = Triangle(a, c, d);
					Triangle triC = Triangle(b, top, c);
					Triangle triD = Triangle(d, bottom, a);

					triA.setTextures({ Vec2({width * i, 0}), Vec2({width * i, 1}), Vec2({(i + 1) * width, 1}) });
					triB.setTextures({ Vec2({width * i, 0}), Vec2({(i + 1) * width, 1}), Vec2({(i + 1) * width, 0}) });

					triA.insert(vertices, indices);
					triB.insert(vertices, indices);
					triC.insert(vertices, indices);
					triD.insert(vertices, indices);
				}

				m_mesh->update(vertices, indices);
			}

		public:

			Cylinder(float radius, float height, int divisions = 100) : 
				m_radius(radius), 
				m_height(height),
				m_divisions(divisions)
			{
				m_mesh = std::make_shared<Mesh>();
				computeMesh();
			}

			int getDivisions() {
				return m_divisions;
			}

			void setDivisions(int divisions) {
				m_divisions = divisions;
				computeMesh();
			}

			float getRadius() {
				return m_radius;
			}

			void setRadius(float radius) {
				m_radius = radius;
				computeMesh();
			}

			float getHeight() {
				return m_height;
			}

			void setHeight(float height) {
				m_height = height;
				computeMesh();
			}

			Mesh* getMesh() {
				return m_mesh.get();
			}
		};
	}
}

#endif