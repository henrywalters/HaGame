#ifndef PRIMITIVES
#define PRIMITIVES 

#include "../Math/Vector.h"
#include "../Utils/Aliases.h"
#include "Mesh.h"
#include <math.h>

namespace hagame {
	namespace graphics {

		const float SR_3 = sqrt(3) / 2.0f;

		const Array<float> QuadVertices = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
		};

		/*const Mesh Triangle = Mesh(
			{
				Vec3({ -SR_3,  -0.5f,  0.0f }),
				Vec3({ 0.0f, 0.5f, 0.0f }),
				Vec3({ SR_3, -0.5f, 0.0f })
			},
			{
				Vec2({ 0.0f, 0.0f }),
				Vec2({ 0.5f, 1.0f }),
				Vec2({ 1.0f, 0.0f }),
			},
			{
				Vec3Int({ 0, 1, 2 })
			}
			);

		const Mesh Cube = Mesh(
			{
				Vec3({ -0.5f,  0.5f, -0.5f }), // Front Face
				Vec3({  0.5f,  0.5f, -0.5f }),
				Vec3({  0.5f, -0.5f, -0.5f }),
				Vec3({ -0.5f, -0.5f, -0.5f }),
				Vec3({ -0.5f,  0.5f,  0.5f }), // Back Face
				Vec3({  0.5f,  0.5f,  0.5f }),
				Vec3({  0.5f, -0.5f,  0.5f }),
				Vec3({ -0.5f, -0.5f,  0.5f }),
				Vec3({  0.5f,  0.5f, -0.5f }), // Right Face
				Vec3({  0.5f,  0.5f,  0.5f }),
				Vec3({  0.5f, -0.5f,  0.5f }),
				Vec3({  0.5f, -0.5f, -0.5f }),
				Vec3({ -0.5f,  0.5f, -0.5f }), // Left Face
				Vec3({ -0.5f,  0.5f,  0.5f }),
				Vec3({ -0.5f, -0.5f,  0.5f }),
				Vec3({ -0.5f, -0.5f, -0.5f }),
				Vec3({ -0.5f,  0.5f, -0.5f }), // Top Face
				Vec3({ -0.5f,  0.5f,  0.5f }),
				Vec3({  0.5f,  0.5f,  0.5f }),
				Vec3({  0.5f,  0.5f, -0.5f }),
				Vec3({ -0.5f,  -0.5f, -0.5f }), // Top Face
				Vec3({ -0.5f,  -0.5f,  0.5f }),
				Vec3({  0.5f,  -0.5f,  0.5f }),
				Vec3({  0.5f,  -0.5f, -0.5f }),
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

				Vec2({ 0.0f, 1.0f }),
				Vec2({ 1.0f, 1.0f }),
				Vec2({ 1.0f, 0.0f }),
				Vec2({ 0.0f, 0.0f }),

				Vec2({ 0.0f, 1.0f }),
				Vec2({ 1.0f, 1.0f }),
				Vec2({ 1.0f, 0.0f }),
				Vec2({ 0.0f, 0.0f }),

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
				Vec3Int({ 8, 9, 10 }), // Right face
				Vec3Int({ 8, 10, 11 }),
				Vec3Int({ 12, 15, 14 }), // Left face
				Vec3Int({ 12, 14, 13 }),
				Vec3Int({ 16, 17, 18 }), // Top face
				Vec3Int({ 16, 18, 19 }),
				Vec3Int({ 20, 22, 21 }), // Bottom face
				Vec3Int({ 20, 22, 23 }),
				Vec3Int({ 0, 1, 2 }), // Front face
				Vec3Int({ 0, 2, 3 }),
				Vec3Int({ 4, 5, 6 }), // Back face
				Vec3Int({ 4, 6, 7 }), 
			});*/
	}
}

#endif