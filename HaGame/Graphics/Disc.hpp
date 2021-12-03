#ifndef DISC_HPP
#define DISC_HPP

#include "Mesh.h"
#include "Triangle.hpp"
#include "DynamicMesh.hpp"
#include "./../Vendor/imgui/imgui.h"
#include <cstdint>

namespace hagame {
	namespace graphics {
		class Disc: public DynamicMesh {
			Ptr<Mesh> m_mesh;
			int m_divisions;
			float m_radius;

			void computeMesh() {
				Array<Vertex> vertices;
				Array<unsigned int> indices;
				unsigned int idx = 0;

				float theta = (2 * PI) / m_divisions;
				float width = (1.0f / m_divisions) * 2 * PI * m_radius;

				for (int i = 0; i < m_divisions; i++) {
					// Add two triangles for top and bottom of cylinder and the face between them

					Vec3 a = Vec3({ cos(i * theta) * m_radius, sin(i * theta) * m_radius, 0.0f });
					Vec3 b = Vec3({ cos((i + 1) * theta) * m_radius , sin((i + 1) * theta) * m_radius, 0.0f });

					Triangle tri = Triangle(a, Vec3::Zero(), b);

					tri.setTextures({ Vec2({width * i, 0}), Vec2({width * i, 1}), Vec2({(i + 1) * width, 1}) });

					tri.insert(vertices, indices);
				}

				m_mesh->update(vertices, indices);
			}

		public:

			Disc(float radius, int divisions = 100) : m_radius(radius), m_divisions(divisions) {
				m_mesh = std::make_shared<Mesh>();
				computeMesh();
			}

			int getDivisions() {
				return m_divisions;
			}

			void setDivisions(int divisions) {
				if (divisions != m_divisions) {
					m_divisions = divisions;
					computeMesh();
				}
				
			}

			float getRadius() {
				return m_radius;
			}

			void setRadius(float radius) {
				if (radius != m_radius) {
					m_radius = radius;
					computeMesh();
				}
			}

			Mesh* getMesh() {
				return m_mesh.get();
			}

			void updateUI() {
				if (ImGui::DragFloat("Radius", &m_radius, 0.1f, 0.0f, 10.0f))
					computeMesh();

				if (ImGui::DragInt("Divisions", &m_divisions, 1, 0, 500))
					computeMesh();
			}
		};
	}
}

#endif
