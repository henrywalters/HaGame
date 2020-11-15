#ifndef MESH
#define MESH

#include <SDL.h>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "../Math/Vector.h"
#include "../Utils/Aliases.h"
#include <iostream>

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

		struct Mesh {
			std::vector<Vec3> vertices;
			std::vector<Vec2> textures;
			std::vector<Vec3> normals;
			std::vector<Vec3Int> indices;

			void calculateNormals() {
				normals.clear();
				for (auto index : indices) {
					Vec3 vec1 = vertices[index[1]] - vertices[index[0]];
					Vec3 vec2 = vertices[index[2]] - vertices[index[0]];
					Vec3 cross = hgVec3(glm::cross(glVec3(vec1), glVec3(vec2)));
					std::cout << cross.toString() << std::endl;
					normals.push_back(cross);
				}
			}

			Mesh() {
				vertices = std::vector<Vec3>();
				textures = std::vector<Vec2>();
				normals = std::vector<Vec3>();
				indices = std::vector<Vec3Int>();
			}

			Mesh(std::vector<Vec3> verts, std::vector<Vec3Int> inds) {
				vertices = verts;
				indices = inds;
				textures = std::vector<Vec2>();
				calculateNormals();
			}

			Mesh(std::vector<Vec3> verts, std::vector<Vec2> texts, std::vector<Vec3Int> inds) {
				textures = texts;
				vertices = verts;
				indices = inds;
				calculateNormals();
			}
		};

		struct GLMesh {
			GLuint vertexBuffer;
			GLuint textureBuffer;
			GLuint normalBuffer;
			GLuint indexBuffer;

			static GLMesh FromMesh(Mesh mesh) {
				GLMesh glMesh; 
				
				glGenBuffers(1, &glMesh.vertexBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, glMesh.vertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(hagame::math::Vector<3, float>), flattenVectorArray<3, float>(mesh.vertices).data(), GL_STATIC_DRAW);

				glGenBuffers(1, &glMesh.normalBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, glMesh.normalBuffer);
				glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(hagame::math::Vector<3, float>), flattenVectorArray<3, float>(mesh.normals).data(), GL_STATIC_DRAW);

				glGenBuffers(1, &glMesh.textureBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, glMesh.textureBuffer);
				glBufferData(GL_ARRAY_BUFFER, mesh.textures.size() * sizeof(Vec2), flattenVectorArray<2, float>(mesh.textures).data(), GL_STATIC_DRAW);

				glGenBuffers(1, &glMesh.indexBuffer);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.indexBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(hagame::math::Vector<3, int>), flattenVectorArray<3, int>(mesh.indices).data(), GL_STATIC_DRAW);

				return glMesh;
			}
		};
	}
}

#endif