#ifndef G_QUAD_HPP
#define G_QUAD_HPP

#include "./Mesh.h"
#include "./Triangle.hpp"

namespace hagame {
	namespace graphics {
		class Quad {
			Ptr<Mesh> m_mesh;
			float m_width, m_height;

			void computeMesh() {
				Array<Vertex> vertices;
				Array<unsigned int> indices;

				Vec3 a = Vec3({ -m_width / 2.0f, -m_height / 2.0f, 0.0f });
				Vec3 b = Vec3({ -m_width / 2.0f, m_height / 2.0f, 0.0f });
				Vec3 c = Vec3({ m_width / 2.0f, m_height / 2.0f, 0.0f });
				Vec3 d = Vec3({ m_width / 2.0f, -m_height / 2.0f, 0.0f });

				Triangle triA = Triangle(a, b, c);
				Triangle triB = Triangle(a, c, d);

				triA.insert(vertices, indices);
				triB.insert(vertices, indices);

				m_mesh->update(vertices, indices);
			}

		public:

			Quad(float width, float height) :
				m_width(width),
				m_height(height) {
				m_mesh = std::make_shared<Mesh>();
				computeMesh();
			}

			Quad(Vec2 size) : Quad(size[0], size[1]) {}

			void setWidth(float width) { m_width = width; computeMesh(); }
			float getWidth() { return m_width; }

			void setHeight(float height) { m_height = height; computeMesh(); }
			float getHeight() { return m_height; }

			Mesh* getMesh() {
				return m_mesh.get();
			}
		};
	}
}

#endif