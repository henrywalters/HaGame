#ifndef LINE_H
#define LINE_H

#include "../Utils/Aliases.h"
#include "Color.h"
#include "ShaderProgram.h"
#include "../Math/Line.h"
#include "./DynamicMesh.hpp"
#include "../Vendor/imgui/imgui.h"
#include "./Triangle.hpp"

namespace hagame {
	namespace graphics {

		class Line : public DynamicMesh {

			Ptr<Mesh> m_mesh;
			Vec3 m_p1;
			Vec3 m_p2;
			float m_thickness;

			void computeMesh() {

				auto delta = m_p2 - m_p1;
				auto zero = Vec3::Zero();
				Vec3 perpY, perpX;

				perpX = delta.normalized().orth();

				perpY = delta.cross(perpX);

				perpY.normalize();

				perpX *= m_thickness * 0.5f;
				perpY *= m_thickness * 0.5f;

				Array<Vertex> vertices;
				Array<unsigned int> indices;

				Vec3 a = m_p1 - perpX - perpY;
				Vec3 b = m_p1 - perpX + perpY;
				Vec3 c = m_p1 + perpX + perpY;
				Vec3 d = m_p1 + perpX - perpY;

				Vec3 e = m_p2 - perpX - perpY;
				Vec3 f = m_p2 - perpX + perpY;
				Vec3 g = m_p2 + perpX + perpY;
				Vec3 h = m_p2 + perpX - perpY;


				Array<Triangle> tris = {
					Triangle(a, b, c), // Front face 0
					Triangle(a, c, d),
					Triangle(e, g, f), // Back face 2
					Triangle(e, h, g),
					Triangle(a, e, b), // Left face 4
					Triangle(e, f, b),
					Triangle(d, c, h), // Right face 6
					Triangle(h, c, g),
					Triangle(b, f, g), // Top face 8
					Triangle(b, g, c),
					Triangle(a, d, e), // bottom face 10
					Triangle(d, h, e)
				};

				tris[4].setTextures({ a.yz(), e.yz(), b.yz() });
				tris[5].setTextures({ e.yz(), f.yz(), b.yz() });
				tris[6].setTextures({ d.yz(), c.yz(), h.yz() });
				tris[7].setTextures({ h.yz(), c.yz(), g.yz() });
				tris[8].setTextures({ b.xz(), f.xz(), g.xz() });
				tris[9].setTextures({ b.xz(), g.xz(), c.xz() });
				tris[10].setTextures({ a.xz(), d.xz(), e.xz() });
				tris[11].setTextures({ d.xz(), h.xz(), e.xz() });

				for (auto tri : tris) {
					tri.insert(vertices, indices);
				}

				m_mesh->update(vertices, indices);
			}

		public:

			Color color;

			Line(Vec3 p1, Vec3 p2, float thickness = 0.1f) : m_p1(p1), m_p2(p2), m_thickness(thickness) {
				m_mesh = std::make_shared<Mesh>();
				computeMesh();
			}

			Line(math::Line line, float thickness = 0.1f): Line(line.a, line.b, thickness) {}

			void setP1(Vec3 p1) { if (m_p1 != p1) { m_p1 = p1; computeMesh(); } }
			void setP2(Vec3 p2) { if (m_p2 != p2) { m_p2 = p2; computeMesh(); } }
			void setThickness(float thickness) { if (thickness != m_thickness) { m_thickness = thickness; computeMesh(); } }

			Vec3 getP1() { return m_p1; }
			Vec3 getP2() { return m_p2; }
			float getThickness() { return m_thickness; }

			Mesh* getMesh() {
				return m_mesh.get();
			}

			void updateUI() {
				if (ImGui::DragFloat3("Point A", m_p1.vector, 0.1f, -10.0f, 10.0f)) {
					computeMesh();
				}

				if (ImGui::DragFloat3("Point B", m_p2.vector, 0.1f, -10.0f, 10.0f)) {
					computeMesh();
				}

				if (ImGui::DragFloat("Thickness", &m_thickness, 0.1f, 0.0f, 10.0f)) {
					computeMesh();
				}
			}
		};
	}
}

#endif
