#ifndef G_QUAD_HPP
#define G_QUAD_HPP

#include "./Mesh.h"
#include "./Triangle.hpp"
#include "./DynamicMesh.hpp"

namespace hagame {
	namespace graphics {
		class Quad : public DynamicMesh {
			Ptr<Mesh> m_mesh;
			Vec2 m_origin;
			Vec2 m_offset;
			float m_width, m_height;

			void computeMesh() {
				Array<Vertex> vertices;
				Array<unsigned int> indices;

				//Vec3 a = Vec3({ -m_width / 2.0f, -m_height / 2.0f, 0.0f });
				//Vec3 b = Vec3({ -m_width / 2.0f, m_height / 2.0f, 0.0f });
				//Vec3 c = Vec3({ m_width / 2.0f, m_height / 2.0f, 0.0f });
				//Vec3 d = Vec3({ m_width / 2.0f, -m_height / 2.0f, 0.0f });

				Vec2 points[4] = {
					Vec2(0, 0) - m_origin + m_offset,
					Vec2(0, m_height) - m_origin + m_offset,
					Vec2(m_width, m_height) - m_origin + m_offset,
					Vec2(m_width, 0) - m_origin + m_offset
				};

				Triangle triA = Triangle(points[0], points[1], points[2]);
				Triangle triB = Triangle(points[0], points[2], points[3]);

				triA.setTextures({ Vec2({0, 1}), Vec2({0, 0}), Vec2({1, 0}) });
				triB.setTextures({ Vec2({0, 1}), Vec2({ 1, 0 }), Vec2({ 1, 1 }) });

				triA.insert(vertices, indices);
				triB.insert(vertices, indices);

				m_mesh->update(vertices, indices);
			}

		public:

			Quad(float width, float height) :
				m_width(width),
				m_height(height),
				m_origin(Vec2(width / 2.0f, height / 2.0f))
			{
				m_mesh = std::make_shared<Mesh>();
				computeMesh();
			}

			Quad(Vec2 size) : Quad(size[0], size[1]) {}

			Quad() : Quad(Vec2(1.0f)) {}

			void setWidth(float width, bool updateOrigin = true) {
				m_width = width;
				if (updateOrigin)
					m_origin[0] = width / 2.0f;
				computeMesh();
			}

			float getWidth() const { return m_width; }

			void setHeight(float height, bool updateOrigin = true) { 
				m_height = height;
				if (updateOrigin)
					m_origin[1] = height / 2.0f;
				computeMesh();
			}

			float getHeight() const { return m_height; }

			void setOrigin(Vec2 origin) { 
				m_origin = origin;
				computeMesh();
			}

			Vec2 getOrigin() const { return m_origin; }

			void setOffset(Vec2 offset) {
				m_offset = offset;
				computeMesh();
			}

			Vec2 getOffset() const { return m_offset; }

			void setSize(Vec2 size, bool updateOrigin = true) {
				m_width = size[0];
				m_height = size[1];
				if (updateOrigin) {
					m_origin[0] = size[0] / 2.0f;
					m_origin[1] = size[1] / 2.0f;
				}
				computeMesh();
			}

			Vec2 getSize() const { return Vec2(m_width, m_height); }

			Mesh* getMesh() {
				return m_mesh.get();
			}

			void updateUI() {

			}
		};
	}
}

#endif