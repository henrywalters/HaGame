#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

namespace hagame {
	namespace graphics {

		// The VertexBuffer class provides an abstraction over the library dependent Vertex Buffer Object. Use a static method to generate a new buffer.
		template <class DataType>
		class VertexBuffer {
		public:
			
			unsigned int id;
			bool dynamic;
			size_t max;
			size_t allocated;

			void initialize() {
				glGenBuffers(1, &id);
			}

			VertexBuffer() : max(0), allocated(0) {}

			~VertexBuffer() {
				glDeleteBuffers(1, &id);
			}

			static Ptr<VertexBuffer> Dynamic(int maxSize) {
				Ptr<VertexBuffer<DataType>> buffer = std::make_shared<VertexBuffer<DataType>>();
				buffer->initialize();
				buffer->dynamic = true;
				buffer->max = maxSize;
				glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(DataType) * maxSize, NULL, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				return buffer;
			}

			static Ptr<VertexBuffer> Dynamic(Array<DataType> data) {
				glCheckError();
				Ptr<VertexBuffer<DataType>> buffer = std::make_shared<VertexBuffer<DataType>>();
				buffer->initialize();
				buffer->dynamic = true;
				buffer->allocated = data.size();
				buffer->max = buffer->allocated;
				glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(DataType) * buffer->max, &data[0], GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glCheckError();
				return buffer;
			}

			static Ptr<VertexBuffer> Static(Array<DataType> data) {
				Ptr<VertexBuffer<DataType>> buffer = std::make_shared<VertexBuffer<DataType>>();
				buffer->initialize();
				buffer->max = data.size();
				buffer->allocated = buffer->max;
				glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(DataType) * data.size(), &data[0], GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				return buffer;
			}

			void clear() {
				bind();
				glClearBufferData(GL_ARRAY_BUFFER, GL_RG32F, GL_RG32F, GL_FLOAT, NULL);
				allocated = 0;
			}

			void bind() {
				glBindBuffer(GL_ARRAY_BUFFER, id);
			}

		};
	}
}

#endif
