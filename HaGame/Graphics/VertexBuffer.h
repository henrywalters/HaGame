#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "./Buffer.hpp"

namespace hagame {
	namespace graphics {

		// The VertexBuffer class provides an abstraction over the library dependent Vertex Buffer Object. Use a static method to generate a new buffer.

		template <class DataType>
		using VertexBuffer = Buffer<DataType, GL_ARRAY_BUFFER>;

		/*template <class DataType>
		class VertexBuffer: public Buffer<DataType, GL_ARRAY_BUFFER> {
		public:
			
			VertexBuffer() : Buffer() {}

			~VertexBuffer() {
				glDeleteBuffers(1, &id);
			}
		};
		*/
	}
}

#endif
