#ifndef PRIMITIVES
#define PRIMITIVES 

#include "../Math/Vector.h"
#include "../Utils/Aliases.h"
#include "Mesh.h"

namespace hagame {
	namespace graphics {
		const Mesh Cube = Mesh(
			{
				Vec3({ -0.5f,  0.5f, -0.5f }),
				Vec3({  0.5f,  0.5f, -0.5f }),
				Vec3({  0.5f, -0.5f, -0.5f }),
				Vec3({ -0.5f, -0.5f, -0.5f }),
				Vec3({ -0.5f,  0.5f, 0.5f }),
				Vec3({  0.5f,  0.5f, 0.5f }),
				Vec3({  0.5f, -0.5f, 0.5f }),
				Vec3({ -0.5f, -0.5f, 0.5f }),
			},
			{
				Vec2({ 0.0f, 1.0f }),
				Vec2({ 1.0f, 1.0f }),
				Vec2({ 1.0f, 0.0f }),
				Vec2({ 0.0f, 0.0f }),
				Vec2({ 0.0f, 1.0f }),
				Vec2({ 1.0f, 1.0f }),
				Vec2({ 1.0f, 0.0f }),
				Vec2({ 0.0f, 0.0f }),
			},
			{
				Vec3Int({ 1, 5, 6 }), // Right face
				Vec3Int({ 1, 6, 2 }),
				Vec3Int({ 4, 0, 3 }), // Left face
				Vec3Int({ 4, 3, 7 }),
				Vec3Int({ 4, 5, 1 }), // Top face
				Vec3Int({ 4, 1, 0 }),
				Vec3Int({ 3, 2, 1 }), // Bottom face
				Vec3Int({ 3, 1, 7 }),
				Vec3Int({ 0, 1, 2 }), // Front face
				Vec3Int({ 0, 2, 3 }),
				Vec3Int({ 4, 5, 6 }), // Back face
				Vec3Int({ 4, 6, 7 }), 
				

			});
	}
}
#endif