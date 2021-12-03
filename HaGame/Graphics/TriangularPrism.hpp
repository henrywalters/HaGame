#ifndef G_TRIANGULAR_PRISM_HPP
#define G_TRIANGULAR_PRISM_HPP

#include "./Mesh.h"
#include "./Triangle.hpp"
#include "./DynamicMesh.hpp"
#include "../Vendor/imgui/imgui.h"

namespace hagame {
	namespace graphics {
		class TriangularPrism : public DynamicMesh {
			Ptr<Mesh> m_mesh;
			Vec3 m_size;

			void computeMesh() {
				Array<Vertex> vertices;
				Array<unsigned int> indices;

				Vec3 a = Vec3({ -m_size[0] / 2.0f, -m_size[1] / 2.0f, -m_size[2] / 2.0f });
				Vec3 b = Vec3({ -m_size[0] / 2.0f, m_size[1] / 2.0f, -m_size[2] / 2.0f });
				// Vec3 c = Vec3({ m_size[0] / 2.0f, m_size[1] / 2.0f, -m_size[2] / 2.0f });
				Vec3 c = Vec3({ m_size[0] / 2.0f, -m_size[1] / 2.0f, -m_size[2] / 2.0f });

				Vec3 e = Vec3({ -m_size[0] / 2.0f, -m_size[1] / 2.0f, m_size[2] / 2.0f });
				Vec3 f = Vec3({ -m_size[0] / 2.0f, m_size[1] / 2.0f, m_size[2] / 2.0f });
				// Vec3 g = Vec3({ m_size[0] / 2.0f, m_size[1] / 2.0f, m_size[2] / 2.0f });
				Vec3 g = Vec3({ m_size[0] / 2.0f, -m_size[1] / 2.0f, m_size[2] / 2.0f });


				Array<Triangle> tris = {
					Triangle(a, b, c), // Front face 0
					Triangle(e, g, f), // Back face 1
					Triangle(a, e, b), // Left face 2
					Triangle(e, f, b), // 3
					Triangle(b, f, c), // Top face 4
					Triangle(c, f, g), // 5
					Triangle(c, e, a), // bottom face 6
					Triangle(g, e, c) // 7
				};

				//tris[4].setTextures({ a.yz(), e.yz(), b.yz() });
				//tris[5].setTextures({ e.yz(), f.yz(), b.yz() });
				//tris[6].setTextures({ d.yz(), c.yz(), h.yz() });
				//tris[7].setTextures({ h.yz(), c.yz(), g.yz() });
				//tris[8].setTextures({ b.xz(), f.xz(), g.xz() });
				//tris[9].setTextures({ b.xz(), g.xz(), c.xz() });
				//tris[10].setTextures({ a.xz(), d.xz(), e.xz() });
				//tris[11].setTextures({ d.xz(), h.xz(), e.xz() });

				for (auto tri : tris) {
					tri.insert(vertices, indices);
				}

				m_mesh->update(vertices, indices);
			}

		public:

			TriangularPrism(Vec3 size) : m_size(size) {
				m_mesh = std::make_shared<Mesh>();
				computeMesh();
			}

			void setSize(Vec3 size) {
				m_size = size;
				computeMesh();
			}

			Vec3 getSize() {
				return m_size;
			}

			Mesh* getMesh() {
				return m_mesh.get();
			}

			void updateUI() {
				if (ImGui::DragFloat3("Size", m_size.vector, 0.1, 0, 10)) {
					computeMesh();
				}
			}
		};
	}
}

#endif