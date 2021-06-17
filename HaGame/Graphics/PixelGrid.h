#ifndef PIXEL_GRID_H
#define PIXEL_GRID_H

#include "./VertexBuffer.h"
#include "./VertexArray.h"
#include "./Pixel.h"

namespace hagame {
	namespace graphics {

		struct PixelInstance {
			Color color;
		};

		class PixelGrid {

			Vec2Int partitions;
			Color clearColor;
			Ptr<VertexBuffer<Vec2>> posBuffer;
			Ptr<VertexBuffer<PixelInstance>> instanceBuffer;
			Ptr<VertexArray> vertArray;
			Vec2 gridSize;
			Vec2 pixelSize;
			Array<Pixel> pixels;

		public:
			
			PixelGrid(Vec2Int _partitions, Vec2 _gridSize, Color _clearColor = Color::white()) : 
				partitions(_partitions),
				gridSize(_gridSize), 
				pixelSize(Vec2({_gridSize[0] / _partitions[0], _gridSize[1] / _partitions[1]})), 
				clearColor(_clearColor),
				pixels(Array<Pixel>())
			{

				Array<Vec2> data;
				Array<Vec4> colorData;
				Array<PixelInstance> instanceData = Array<PixelInstance>();

				for (int i = 0; i < partitions[1]; i++) {
					
					for (int j = 0; j < partitions[0]; j++) {
						Pixel pixel;

						pixel.color = (i * partitions[1] + (j + i)) % 2 == 0 ? Color::red() : clearColor ;
						pixel.pos = Vec2({ j * pixelSize[0] + pixelSize[0] / 2, i * pixelSize[1] + pixelSize[1] / 2 });

						pixels.push_back(pixel);

						Vec2 a, b, c, d;

						a = pixel.pos + Vec2({ -pixelSize[0] / 2, -pixelSize[1] / 2 });
						b = pixel.pos + Vec2({ pixelSize[0] / 2, -pixelSize[1] / 2 });
						c = pixel.pos + Vec2({ pixelSize[0] / 2, pixelSize[1] / 2 });
						d = pixel.pos + Vec2({ -pixelSize[0] / 2, pixelSize[1] / 2 });

						data.insert(data.end(), { a, b, c, a, c, d });


						PixelInstance instance;
						instance.color = pixel.color;
						//instance.index = i;
						instanceData.insert(instanceData.end(), { instance, instance, instance, instance, instance, instance });
					}
				}

				vertArray = std::make_shared<VertexArray>();
				vertArray->initialize();

				std::cout << instanceData.size() << std::endl;

				posBuffer = VertexBuffer<Vec2>::Dynamic(data);
				instanceBuffer = VertexBuffer<PixelInstance>::Dynamic(instanceData);

				vertArray->defineAttribute<Vec2>(posBuffer.get(), DataType::Float, 0, 2);
				vertArray->defineAttribute<PixelInstance>(instanceBuffer.get(), DataType::Float, 1, 4);
				
				//buffer->defineAttrib(0, 2, sizeof(Vec2), (void *) 0);
			}

			void draw(ShaderProgram* shader) {
				shader->use();
				vertArray->bind();
				glDrawArrays(GL_TRIANGLES, 0, partitions[1] * partitions[0] * 6);
			}

		};
	}
}

#endif
