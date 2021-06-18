#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "./VertexBuffer.h"
#include "./DataType.h"

namespace hagame {
	namespace graphics {

		class VertexArray {
		public:

			unsigned int id;

			VertexArray() {}

			~VertexArray() {
				glDeleteVertexArrays(1, &id);
			}

			void initialize() {
				glGenVertexArrays(1, &id);
			}

			void bind() {
				glBindVertexArray(id);
			}


			/// <summary> Defines an attribute of the Vertex Array. Example: A 3D position vector is an attribute</summary>
			/// <param name="layoutNumber">The layout number in the shader program.</param>
			/// <param name="attributeSize">The number of data members in the attribute. The example would be 3</param>
			template <class ChunkDataType>
			void defineAttribute(VertexBuffer<ChunkDataType>* buffer, DataType dataType, int layoutNumber, size_t attributeSize) {
				buffer->bind();
				bind();
				glEnableVertexAttribArray(layoutNumber);
				glVertexAttribPointer(layoutNumber, attributeSize, GL_FLOAT, GL_FALSE, sizeof(ChunkDataType), (void *) 0);
				glBindVertexArray(0);
			}

			/// <summary> Defines an attribute of the Vertex Array. Example: A 3D position vector is an attribute</summary>
			/// <param name="layoutNumber">The layout number in the shader program.</param>
			/// <param name="attributeSize">The number of data members in the attribute. The example would be 3</param>
			/// <param name="chunkSize">The total size of each chunk of buffer data. </param>
			/// <param name="chunkOffset">The offset of the attribute in each data chunk. </param>
			template <class ChunkDataType>
			void defineAttribute(VertexBuffer<ChunkDataType>* buffer, DataType dataType, int layoutNumber, size_t attributeSize, size_t chunkOffset) {
				buffer->bind();
				bind();
				glEnableVertexAttribArray(layoutNumber);
				glVertexAttribPointer(layoutNumber, attributeSize, GL_FLOAT, GL_FALSE, sizeof(ChunkDataType), (void*)chunkOffset);
				glBindVertexArray(0);
			}

			void setInstanced(int layoutNumber) {
				bind();
				glEnableVertexAttribArray(layoutNumber);
				glVertexAttribDivisor(layoutNumber, 1);
				glBindVertexArray(0);
			}
		};
	}
}

#endif
