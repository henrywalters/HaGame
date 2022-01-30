#ifndef LINE_BUFFER_HPP
#define LINE_BUFFER_HPP

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "../Math/Line.h"
#include "../Utils/Aliases.h"
#include "Color.h"
#include "RectPrism.hpp"

namespace hagame {
	namespace graphics {

		struct BatchLine {
			Mat4 transform;
			Color color;
		};

		class LineBuffer {
		public:

			const size_t MAX_LINE_COUNT = 10000;

			LineBuffer() {}

			// Create buffers - this must be called before any other methods are used.
			void initializeBuffers();

			// Remove all lines from the buffer and reset line count
			void clear();

			// Computes the necessary transform matrix and add it to the buffer
			void insert(math::Line line, Color color, float thickness = 0.05f);

			// Render all of the lines in the buffer. Optionally clear the buffer after drawing for convienence 
			void draw(bool clearBuffer = true);

			size_t getLineCount() { return m_lineCount; }

		private:
			Ptr<VertexBuffer<BatchLine>> m_lineBuffer;
			Ptr<RectPrism> m_cube;
			size_t m_lineCount = 0;
		};
	}
}

#endif
