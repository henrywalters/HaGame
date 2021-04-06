#ifndef MESH
#define MESH

#include <SDL.h>
#include <vector>
#include <iostream>
#include "../Math/Vector.h"
#include "../Utils/Aliases.h"
#include "../Utils/File.h"
#include "../Utils/String.h"
#include "../Graphics/ShaderProgram.h"

namespace hagame {
	namespace graphics {

		template <size_t size, class T>
		std::vector<T> flattenVectorArray(std::vector<hagame::math::Vector<size, T>> vectArr) {
			std::vector<T> out = std::vector<T>();
			for (auto vect : vectArr) {
				for (int i = 0; i < size; i++) {
					out.push_back(vect[i]);
				}
			}
			return out;
		}

		struct Vertex {
			Vec3 position;
			Vec3 normal;
			Vec2 texCoords;
		};

		class Mesh {

		private:

			unsigned int VAO, VBO, EBO;

			void initializeForGL() {
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);

				glBindVertexArray(VAO);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

				glBindVertexArray(0);
			}

		public:
			Array<Vertex> vertices;
			Array<unsigned int> indices;

			Mesh() {
				vertices = Array<Vertex>();
				indices = Array<unsigned int>();
			}

			void draw(ShaderProgram* shader) {
				shader->use();
				glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}

			Cube getBoundingCube() {
				bool init = false;
				Vec3 min, max;

				for (auto v : vertices) {
					if (!init) {
						init = true;
						min = v.position;
						max = v.position;
					}
					else {
						for (int i = 0; i < 3; i++) {
							if (v.position[i] < min[i]) {
								min[i] = v.position[i];
							}

							if (v.position[i] > max[i]) {
								max[i] = v.position[i];
							}
						}
					}
				}

				return Cube(min, max - min);
			}

			static Mesh FromOBJ(utils::File* file) {
				Mesh mesh;
				Array<Vec3> positions = Array<Vec3>();
				Array<Vec2> textures = Array<Vec2>();
				Array<Vec3> normals = Array<Vec3>();

				unsigned int idx = 0;

				for (auto line : file->readLines()) {
					auto parts = stringSplit(line, ' ');

					if (parts.size() == 0) continue;

					if (parts[0] == "v") {
						positions.push_back(Vec3({ stof(parts[1]), stof(parts[2]), stof(parts[3]) }));
					}

					if (parts[0] == "vt") {
						textures.push_back(Vec2({ stof(parts[1]), stof(parts[2]) }));
					}

					if (parts[0] == "vn") {
						normals.push_back(Vec3({ stof(parts[1]), stof(parts[2]), stof(parts[3]) }));
					}

					if (parts[0] == "f") {

						auto f1 = stringSplit(parts[1], '/');
						auto f2 = stringSplit(parts[2], '/');
						auto f3 = stringSplit(parts[3], '/');

						Vertex v1;
						Vertex v2;
						Vertex v3;
						
						v1.position = positions[stoi(f1[0]) - 1];
						v2.position = positions[stoi(f2[0]) - 1];
						v3.position = positions[stoi(f3[0]) - 1];

						v1.texCoords = textures[stoi(f1[1]) - 1];
						v1.texCoords = textures[stoi(f2[1]) - 1];
						v1.texCoords = textures[stoi(f3[1]) - 1];

						v1.normal = normals[stoi(f1[2]) - 1];
						v2.normal = normals[stoi(f2[2]) - 1];
						v2.normal = normals[stoi(f2[2]) - 1];

						mesh.vertices.insert(mesh.vertices.end(), { v1, v2, v3 });
						mesh.indices.insert(mesh.indices.end(), { idx, idx + 1, idx + 2 });
						idx += 3;

						if (parts.size() == 4) {
							auto f4 = stringSplit(parts[3], '/');
							Vertex v4;
							v4.position = positions[stoi(f4[0]) - 1];
							v4.texCoords = textures[stoi(f4[1]) - 1];
							v4.normal = normals[stoi(f4[2]) - 1];

							mesh.vertices.insert(mesh.vertices.end(), { v1, v4, v3 });
							mesh.indices.insert(mesh.indices.end(), { idx, idx + 1, idx + 2 });
							idx += 3;
						}
					}
				}
				mesh.initializeForGL();
				return mesh;
			}
		};
	}
}

#endif