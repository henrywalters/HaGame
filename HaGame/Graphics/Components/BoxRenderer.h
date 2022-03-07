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
			float fillPercent = 1.0f;
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

			void draw(Vec3 position) {
				shader->use();

				if (borderColor.has_value()) {
					shader->setVec4("color", borderColor.value());
					shader->setMat4("model", Mat4::Translation(position + box.pos.resize<3>()) * Mat4::Scale(box.size.resize<3>()));
					vao->bind();
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glCheckError();
				}

				if (fillColor.has_value()) {

					auto fillSize = box.size.resize<3>();
					fillSize[0] *= fillPercent;
					// auto fillOffset = Vec3({ -(box.size[0] - (1 - fillPercent)) / 2.0f, 0.0f, 0.0f });

					// std::cout << fillSize.toString() << "\n";

					shader->setVec4("color", fillColor.value());
					shader->setMat4("model", Mat4::Translation(position + box.pos.resize<3>()) * Mat4::Scale(fillSize));
					// shader->setMat4("model", Mat4::Translation(position + box.pos.resize<3>()) * Mat4::Scale(box.size.resize<3>()));
					vao->bind();
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glCheckError();
				}

				glBindVertexArray(0);

				glBindVertexArray(0);
			}
		};
	}
}

#endif