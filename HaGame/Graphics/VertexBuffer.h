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
				buffer->dynamic = false;
				buffer->max = data.size();
				buffer->allocated = buffer->max;
				glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(DataType) * data.size(), &data[0], GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				return buffer;
			}

			void canAssign(unsigned int index, size_t size) {
				if (index + size >= allocated) {
					throw new std::exception("Can not assign data of this size to this index in buffer. Larger than allocated memory size");
				}
			}

			// Update a single instance of the DataType at a given index
			void update(unsigned int index, DataType data) {
				canAssign(index, sizeof(DataType));
				bind();
				glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(DataType), sizeof(DataType), &data);
				unbind();
			}

			// Update several instances of the DataType starting at a given index
			void update(unsigned int index, Array<DataType> data) {
				canAssign(index, sizeof(DataType) * data.size());
				bind();
				glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(DataType), sizeof(DataType) * data.size(), &data[0]);
				unbind();
			}

			// Update a member of a single instance at a given index. Useful for updating a member of a struct or class
			template <class MemberType>
			void update(unsigned int index, size_t memberOffset, MemberType memberData) {
				canAssign(index, sizeof(MemberType));
				bind();
				glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(DataType) + memberOffset, sizeof(MemberType), &memberData);
				unbind();
			}

			void clear() {
				bind();
				glClearBufferData(GL_ARRAY_BUFFER, GL_RG32F, GL_RG32F, GL_FLOAT, NULL);
				allocated = 0;
			}

			void bind() {
				glBindBuffer(GL_ARRAY_BUFFER, id);
			}

			void unbind() {
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		};
	}
}

#endif
