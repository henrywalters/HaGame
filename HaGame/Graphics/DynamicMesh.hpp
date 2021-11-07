#ifndef DYNAMIC_MESH_HPP
#define DYNAMIC_MESH_HPP

#include "./Mesh.h"

namespace hagame {
	namespace graphics {
		class DynamicMesh {
		public:
			virtual Mesh* getMesh() {
				throw new std::exception("getMesh not implemented");
			}

			virtual void updateUI() {
				throw new std::exception("updateUI not implemented");
			}
		};
	}
}

#endif
