#ifndef PIXEL_GRID_H
#define PIXEL_GRID_H

#include "./VertexBuffer.h"
#include "./VertexArray.h"
#include "./Pixel.h"

namespace hagame {
	namespace graphics {

		struct PixelInstance {
			Vec2 pos;
			Color color;
		};

		class PixelGrid {

			Ptr<VertexBuffer<Vec2>> quadBuffer;
			Ptr<VertexBuffer<Pixel>> instanceBuffer;
			Ptr<VertexArray> vertArray;

			Array<Pixel> pixels;
			Vec2 padding;

		public:

			Vec2 gridSize;
			Vec2 pixelSize;
			Vec2Int partitions;
			Color clearColor;
			
			PixelGrid(Vec2Int _partitions, Vec2 _gridSize, Color _clearColor = Color::white(), Vec2 _padding = Vec2::Zero()) : 
				partitions(_partitions),
				gridSize(_gridSize), 
				pixelSize(Vec2({_gridSize[0] / _partitions[0], _gridSize[1] / _partitions[1]})), 
				clearColor(_clearColor),
				padding(_padding),
				pixels(Array<Pixel>())
			{

				auto scale = Vec2::Identity() - padding;

				Array<Vec2> quadData = {
					Vec2({-0.5f,  0.5f}).prod(pixelSize).prod(scale),
					Vec2({ 0.5f, -0.5f}).prod(pixelSize).prod(scale),
					Vec2({-0.5f, -0.5f}).prod(pixelSize).prod(scale),

					Vec2({-0.5f,  0.5f}).prod(pixelSize).prod(scale),
					Vec2({ 0.5f, -0.5f}).prod(pixelSize).prod(scale),
					Vec2({ 0.5f,  0.5f}).prod(pixelSize).prod(scale)
				};

				

				quadBuffer = VertexBuffer<Vec2>::Static(quadData);

				for (int i = 0; i < partitions[1]; i++) {
					
					for (int j = 0; j < partitions[0]; j++) {
						Pixel pixel;
						pixel.color = clearColor;
						pixel.pos = getPos(Vec2Int({j, i}));
						pixels.push_back(pixel);
					}
				}

				vertArray = std::make_shared<VertexArray>();
				vertArray->initialize();

				instanceBuffer = VertexBuffer<Pixel>::Dynamic(pixels);

				vertArray->defineAttribute<Vec2>(quadBuffer.get(), DataType::Float, 0, 2);

				vertArray->defineAttribute<Pixel>(instanceBuffer.get(), DataType::Float, 1, 2, offsetof(Pixel, pos));
				vertArray->setInstanced(1);

				vertArray->defineAttribute<Pixel>(instanceBuffer.get(), DataType::Float, 2, 4, offsetof(Pixel, color));
				vertArray->setInstanced(2);

			}

			Vec2 getPos(Vec2Int index) {
				auto paddingOffset = pixelSize.prod(padding);
				return index.cast<float>().prod(pixelSize) + pixelSize / 2 + paddingOffset / 2;
			}

			Vec2Int getIndex(Vec2 pos) {
				auto paddingOffset = pixelSize.prod(padding);
				return (pos - paddingOffset / 2).div(pixelSize).cast<int>();
			}

			void setColor(int row, int col, Color color) {
				if (row >= 0 && col >= 0 && row < partitions[1] && col < partitions[0]) {
					instanceBuffer->update<Color>(row * partitions[0] + col, offsetof(Pixel, color), color);
				}
			}

			void draw(ShaderProgram* shader) {
				shader->use();
				vertArray->bind();
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, partitions[0] * partitions[1]);
			}

		};
	}
}

#endif
