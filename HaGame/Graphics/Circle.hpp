#ifndef G_CIRCLE_HPP
#define G_CIRCLE_HPP

#include "./Mesh.h"

namespace hagame {
	namespace graphics {
		class Circle {
			float m_radius;
			unsigned int m_divisons;
			Ptr<Mesh> m_mesh;

			void computeMesh() {

			}

		public:

			Circle(float radius) : m_radius(radius) {
				m_mesh = std::make_shared<Mesh>();
				computeMesh();
			}

		};
	}
}

#endif
