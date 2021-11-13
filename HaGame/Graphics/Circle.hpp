#ifndef G_CIRCLE_HPP
#define G_CIRCLE_HPP

#include "./Mesh.h"
#include "./DynamicMesh.hpp"
#include "./Triangle.hpp"
#include "./../Vendor/imgui/imgui.h"

namespace hagame {
	namespace graphics {
		class Circle : public DynamicMesh {
			float m_radius;
			float m_thickness = .025f;
			int m_divisions = 16;
			Ptr<Mesh> m_mesh;

			void computeMesh() {
				Array<Vertex> vertices;
				Array<unsigned int> indices;

				float theta = (2 * PI) / m_divisions;
				float width = (1.0f / m_divisions) * 2 * PI * m_radius;

				for (int i = 0; i <= m_divisions; i++) {

					float innerRadius = m_radius - m_thickness * 0.5f;
					float outerRadius = m_radius + m_thickness * 0.5f;

					Vec3 top = Vec3::Top(m_thickness * 0.5f);

					Vec3 A = Vec3({ cos(i * theta) * innerRadius, 0.0f, sin(i * theta) * innerRadius });
					Vec3 B = Vec3({ cos((i + 1) * theta) * innerRadius , 0.0f, sin((i + 1) * theta) * innerRadius });
					Vec3 C = Vec3({ cos((i + 1) * theta) * outerRadius , 0.0f, sin((i + 1) * theta) * outerRadius });
					Vec3 D = Vec3({ cos(i * theta) * outerRadius, 0.0f, sin(i * theta) * outerRadius });

					Vec3 a = A + top;
					Vec3 b = B + top;
					Vec3 c = C + top;
					Vec3 d = D + top;
					Vec3 e = A - top;
					Vec3 f = B - top;
					Vec3 g = C - top;
					Vec3 h = D - top;

					Triangle triA = Triangle(a, b, c);
					Triangle triB = Triangle(a, c, d);
					Triangle triC = Triangle(g, f, e);
					Triangle triD = Triangle(h, g, e);
					Triangle triE = Triangle(e, f, b);
					Triangle triF = Triangle(e, b, a);
					Triangle triG = Triangle(c, g, h);
					Triangle triH = Triangle(d, c, h);

					triA.insert(vertices, indices);
					triB.insert(vertices, indices);
					triC.insert(vertices, indices);
					triD.insert(vertices, indices);
					triE.insert(vertices, indices);
					triF.insert(vertices, indices);
					triG.insert(vertices, indices);
					triH.insert(vertices, indices);
				}

				m_mesh->update(vertices, indices);
			}

		public:

			Circle(float radius) : m_radius(radius) {
				m_mesh = std::make_shared<Mesh>();
				computeMesh();
			}

			void setRadius(float radius) { m_radius = radius; computeMesh(); }
			float getRadius() { return m_radius; }

			void setDivisions(int divisions) { m_divisions = divisions; computeMesh(); }
			int getDivisions() { return m_divisions; }

			void setThickness(float thickness) { m_thickness = thickness; computeMesh(); }
			float getThickness() { return m_thickness; }

			Mesh* getMesh() {
				return m_mesh.get();
			}

			void updateUI() {
				if (ImGui::DragFloat("Radius", &m_radius, 0.1f, 0.0f, 10.0f))
					computeMesh();

				if (ImGui::DragFloat("Thickness", &m_thickness, 0.1f, 0.0f, 5.0f))
					computeMesh();

				if (ImGui::DragInt("Divisions", &m_divisions, 1.0f, 0, 1000))
					computeMesh();
			}
		};
	}
}

#endif
