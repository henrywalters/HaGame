#ifndef MESH
#define MESH

#include <SDL.h>
#include <vector>
#include <iostream>
#include "../Math/Vector.h"
#include "../Utils/Aliases.h"
#include "../Utils/File.h"
#include "../Utils/String.h"
#include "../Utils/SpatialMap.h"
#include "../Graphics/ShaderProgram.h"
#include "../Math/Triangle.h"

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

		struct Face {
			Vertex vertices[3];
			int index;
		};

		struct MeshDefinition {
			Array<Vertex> vertices;
			Array<unsigned int> indices;
		};

		struct ConcatMeshDefinition {
			MeshDefinition mesh;
			Mat4 model;
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

				glDisableVertexAttribArray(0);
				//glDisableVertexAttribArray(1);
				//glDisableVertexAttribArray(2);

			}

			void removeBuffers() {
				glDeleteVertexArrays(1, &VAO);
				glDeleteBuffers(1, &VBO);
				glDeleteBuffers(1, &EBO);
			}

		public:
			Array<Vertex> vertices;
			Array<unsigned int> indices;

			Mesh() {
				vertices = Array<Vertex>();
				indices = Array<unsigned int>();
			}

			Mesh(hagame::utils::File* file) {
				vertices = Array<Vertex>();
				indices = Array<unsigned int>();
				loadOBJ(file);
			}

			Mesh(Array<Vertex> _vertices, Array<unsigned int> _indices): vertices(_vertices), indices(_indices) {
				initializeForGL();
			}

			Mesh(MeshDefinition definition) : vertices(definition.vertices), indices(definition.indices) {
				initializeForGL();
			}

			Mesh(Array<ConcatMeshDefinition> meshDefs) {
				auto faces = Array<Ptr<Face>>();
				vertices = Array<Vertex>();
				indices = Array<unsigned int>();
				unsigned int index = 0;

				auto pointMap = hagame::utils::SpatialMap<Face, float>(Vec3({ 0.1, 0.1, 0.1}));

				for (auto meshDef : meshDefs) {
					for (int i = 0; i < meshDef.mesh.indices.size() / 3; i++) {

						int i1 = meshDef.mesh.indices[(i * 3)];
						int i2 = meshDef.mesh.indices[(i * 3) + 1];
						int i3 = meshDef.mesh.indices[(i * 3) + 2];

						Vertex v1 = meshDef.mesh.vertices[i1];
						v1.position = (meshDef.model * v1.position.resize<4>(1.0f)).resize<3>();
						Vertex v2 = meshDef.mesh.vertices[i2];
						v2.position = (meshDef.model * v2.position.resize<4>(1.0f)).resize<3>();
						Vertex v3 = meshDef.mesh.vertices[i3];
						v3.position = (meshDef.model * v3.position.resize<4>(1.0f)).resize<3>();

						Ptr<Face> face = std::make_shared<Face>();
						face->vertices[0] = v1;
						face->vertices[1] = v2;
						face->vertices[2] = v3;
						face->index = i;
						pointMap.insert(v1.position, face);
						pointMap.insert(v2.position, face);
						pointMap.insert(v3.position, face);
						faces.push_back(face);

						//vertices.insert(vertices.end(), { v1, v2, v3 });
						//indices.insert(indices.end(), { index, index + 1, index + 2 });
						//index += 3;
					}
				}

				std::cout << "Original mesh size: " << faces.size() << " triangles\n";
				int culled = 0;

				for (int i = 0; i < faces.size(); i++) {

					bool showFace = true;

					auto faceTri = hagame::math::Triangle(faces[i]->vertices[0].position, faces[i]->vertices[1].position, faces[i]->vertices[2].position);

					for (int j = 0; j < 3; j++) {
						for (auto face : pointMap.get(faces[i]->vertices[j].position)) {
							auto otherTri = hagame::math::Triangle(face->vertices[0].position, face->vertices[1].position, face->vertices[2].position);
							
							if (faces[i]->index != face->index && faces[i]->vertices[0].normal == face->vertices[0].normal * -1) {
								if (faceTri == otherTri) {
									showFace = false;
									culled++;
									break;
								}
							}
						}

						if (!showFace)
							break;
					}

					if (showFace) {
						vertices.insert(vertices.end(), { faces[i]->vertices[0], faces[i]->vertices[1], faces[i]->vertices[2] });
						indices.insert(indices.end(), { index, index + 1, index + 2 });
						index += 3;
					}
					
				}

				std::cout << "\nRemoved " << culled << ".\n";

				initializeForGL();
			}

			~Mesh() {
				removeBuffers();
			}

			void update(Array<Vertex> _vertices, Array<unsigned int> _indices) {
				// TODO: Update buffers instead of regenerating. Need to track if size is increased
				vertices = _vertices;
				indices = _indices;
				removeBuffers();
				initializeForGL();
			}

			MeshDefinition getDefinition() {
				return MeshDefinition{
					vertices,
					indices
				};
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


			void loadOBJ(utils::File* file) {
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
						v2.texCoords = textures[stoi(f2[1]) - 1];
						v3.texCoords = textures[stoi(f3[1]) - 1];

						v1.normal = normals[stoi(f1[2]) - 1];
						v2.normal = normals[stoi(f2[2]) - 1];
						v3.normal = normals[stoi(f2[2]) - 1];

						vertices.insert(vertices.end(), { v1, v2, v3 });
						indices.insert(indices.end(), { idx, idx + 1, idx + 2 });
						idx += 3;

						if (parts.size() == 5) {
							auto f4 = stringSplit(parts[4], '/');
							Vertex v4;
							v4.position = positions[stoi(f4[0]) - 1];
							v4.texCoords = textures[stoi(f4[1]) - 1];
							v4.normal = normals[stoi(f4[2]) - 1];

							vertices.insert(vertices.end(), { v1, v3, v4});
							indices.insert(indices.end(), { idx, idx + 1, idx + 2 });
							idx += 3;
						}
					}
				}
				initializeForGL();
			}
		};
	}
}

#endif