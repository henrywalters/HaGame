#ifndef GRID_LINES_H
#define GRID_LINES_H

#include "../Utils/Aliases.h"
#include "./Color.h"
#include "./VertexBuffer.h"
#include "./VertexArray.h"

namespace hagame {
	namespace graphics {
		class GridLines {
			Ptr<VertexBuffer<Vec2>> lineBufferX, lineBufferY;
			Ptr<VertexBuffer<Vec2>> instanceBufferX, instanceBufferY;
			Ptr<VertexArray> vertX, vertY;

			Rect rect;

			Vec2Int partitions;
			Vec2 pixelSize;

			bool needsRefresh = false;

		public:

			float lineWidth;
			
			Color color;

			GridLines(Rect _rect, Vec2Int _partitions, Color _color, float _lineWidth = 1.0f) :
				rect(_rect),
				partitions(_partitions),
				color(_color),
				lineWidth(_lineWidth),
				pixelSize(_rect.size.div(_partitions.cast<float>()))
			{
				lineBufferX = VertexBuffer<Vec2>::Dynamic(6);
				lineBufferY = VertexBuffer<Vec2>::Dynamic(6);

				instanceBufferX = VertexBuffer<Vec2>::Dynamic(partitions[0]);
				instanceBufferY = VertexBuffer<Vec2>::Dynamic(partitions[1]);

				vertX = std::make_shared<VertexArray>();
				vertY = std::make_shared<VertexArray>(); 

				vertX->initialize();
				vertX->defineAttribute<Vec2>(lineBufferX.get(), DataType::Float, 0, 2);
				vertX->defineAttribute<Vec2>(instanceBufferX.get(), DataType::Float, 1, 2);
				vertX->setInstanced(1);

				vertY->initialize();
				vertY->defineAttribute<Vec2>(lineBufferY.get(), DataType::Float, 0, 2);
				vertY->defineAttribute<Vec2>(instanceBufferY.get(), DataType::Float, 1, 2);
				vertY->setInstanced(1);
			}

			void setSize(Vec2 size) {
				rect.size = size;
				pixelSize = rect.size.div(partitions.cast<float>());
				needsRefresh = true;
			}

			void setPartitions(Vec2Int _partitions) {
				partitions = _partitions;
				pixelSize = rect.size.div(partitions.cast<float>());
				needsRefresh = true;
			}

			void updateLineBuffers() {
				Array<Vec2> lineDataX = {
					Vec2({-rect.size[0] / 2, -lineWidth / 2}),
					Vec2({ rect.size[0] / 2, lineWidth / 2}),
					Vec2({-rect.size[0] / 2, lineWidth / 2}),

					Vec2({-rect.size[0] / 2, -lineWidth / 2}),
					Vec2({rect.size[0] / 2, -lineWidth / 2}),
					Vec2({ rect.size[0] / 2, lineWidth / 2}),
				};

				Array<Vec2> lineDataY = {
					Vec2({-lineWidth / 2, -rect.size[1] / 2}),
					Vec2({lineWidth / 2, rect.size[1] / 2}),
					Vec2({-lineWidth / 2, rect.size[1] / 2}),

					Vec2({-lineWidth / 2, -rect.size[1] / 2}),
					Vec2({lineWidth / 2, -rect.size[1] / 2}),
					Vec2({lineWidth / 2, rect.size[1] / 2}),
				};

				lineBufferX->update(0, lineDataX);
				lineBufferY->update(0, lineDataY);
			}

			void updateInstanceBuffers() {

				instanceBufferX->bind();
				instanceBufferX->resize(partitions[1] + 1);
				
				for (int i = 0; i <= partitions[1]; i++) {
					instanceBufferX->update(i, rect.pos + Vec2({ 0, (float)i * pixelSize[1] - rect.size[1] / 2.0f }));
				}
				instanceBufferY->bind();
				instanceBufferY->resize(partitions[0] + 1);

				for (int i = 0; i <= partitions[0]; i++) {
					instanceBufferY->update(i, rect.pos + Vec2({ (float)i * pixelSize[0] - rect.size[0] / 2.0f, 0 }));
				}
			}

			void draw(ShaderProgram* shader) {
				shader->use();
				shader->setVec4("color", color);
				shader->setMat4("model", Mat4::Identity());

				if (needsRefresh) {
					std::cout << rect.toString() << "\n";
					updateLineBuffers();
					updateInstanceBuffers();
					needsRefresh = false;
				}
				
				vertX->bind();
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, partitions[1] + 1);
				vertY->bind();
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, partitions[0] + 1);
			}
		};
	}
}

#endif
