#ifndef BOX_RENDERER_H
#define BOX_RENDERER_H

#include "./../../Utils/Aliases.h"
#include "./../Shader.h"
#include "./../ShaderProgram.h"
#include "./../VertexBuffer.h"
#include "./../VertexArray.h"
#include "./../Color.h"

namespace hagame {
	namespace graphics {
		struct BoxRenderer {
			ShaderProgram* shader;
			Rect box;
			Optional<Vec4> fillColor;
			Optional<Vec4> borderColor;

			Ptr<VertexBuffer<Vec4>> vbo;
			Ptr<VertexArray> vao;

			BoxRenderer() {
				vbo = VertexBuffer<Vec4>::Static(QuadVertexVectors);
				vao = std::make_shared<VertexArray>();
				vao->initialize();
				vao->defineAttribute<Vec4>(vbo.get(), DataType::Float, 0, 4);
			}

			~BoxRenderer() {
				vbo->clear();
			}

			void draw() {
				shader->use();
				if (fillColor.has_value()) {
					vao->bind();
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glCheckError();
					glBindVertexArray(0);
				}
			}
		};
	}
}

#endif