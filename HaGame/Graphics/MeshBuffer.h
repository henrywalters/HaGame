#ifndef MESH_BUFFER_HPP
#define MESH_BUFFER_HPP

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "../Utils/Aliases.h"
#include "Mesh.h"


namespace hagame {
	namespace graphics {

		struct MeshTransform {
			Mat4 pos;
			Mat4 rot;
			Mat4 scale;
		};

		// Store many instances of a single mesh in a buffer for very fast rendering. Initialize buffers before usage!
		class MeshBuffer {
		public:
			const size_t MAX_MESH_COUNT = 100000;

			MeshBuffer();

			// Create buffers - this must be called before any other methods are used.
			void initializeBuffers(Mesh* mesh);

			// Remove all mesh instances from buffer
			void clear();

			// Insert a new instance into the buffer. Returns the index of the newly created instance
			int insert(Mat4 pos, Mat4 rot, Mat4 scale);

			// Update a single instance in the buffer.
			void update(int index, Mat4 pos, Mat4 rot, Mat4 scale);

			// Return the transform matrix at the given index
			MeshTransform get(int index);

			// Draw all instances, and optionally, clear the buffer after drawing
			void draw(bool clearBuffer = true);

			size_t getMeshCount() { return m_meshCount; }
		private:

			Ptr<VertexBuffer<MeshTransform>> m_meshBuffer;
			Mesh* m_mesh;
			size_t m_meshCount = 0;
		};
	}
}

#endif