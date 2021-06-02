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
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
		};

		const MeshDefinition QuadMesh = MeshDefinition{ 
			{
				Vertex{ Vec3({ 0.0f, 0.0f, 0.0f }), Vec3::Top(), Vec2::Zero() },
				Vertex{ Vec3({ 0.0f, 1.0f, 0.0f }), Vec3::Top(), Vec2({0.0f, 1.0f}) },
				Vertex{ Vec3({ 1.0f, 1.0f, 0.0f }), Vec3::Top(), Vec2({1.0f, 1.0f}) },
				Vertex{ Vec3({ 1.0f, 0.0f, 0.0f }), Vec3::Top(), Vec2({1.0f, 0.0f}) }
			}, 
			{
				0, 1, 2, 0, 2, 3
			} 
		};

		const MeshDefinition CubeMesh = MeshDefinition{
			{
				Vertex{ Vec3({ -0.5f, 0.5f, 0.5f }), Vec3::Face(), Vec2::Zero() }, // Face
				Vertex{ Vec3({ 0.5f, 0.5f, 0.5f }), Vec3::Face(), Vec2({0.0f, 1.0f}) },
				Vertex{ Vec3({ 0.5f, -0.5f, 0.5f }), Vec3::Face(), Vec2({1.0f, 1.0f}) },
				Vertex{ Vec3({ -0.5f, -0.5f, 0.5f }), Vec3::Face(), Vec2({1.0f, 0.0f}) },
				Vertex{ Vec3({ -0.5f, 0.5f, -0.5f }), Vec3::Back(), Vec2::Zero() }, // Back
				Vertex{ Vec3({ 0.5f, 0.5f, -0.5f }), Vec3::Back(), Vec2({0.0f, 1.0f}) },
				Vertex{ Vec3({ 0.5f, -0.5f, -0.5f }), Vec3::Back(), Vec2({1.0f, 1.0f}) },
				Vertex{ Vec3({ -0.5f, -0.5f, -0.5f }), Vec3::Back(), Vec2({1.0f, 0.0f}) },
				Vertex{ Vec3({ -0.5f, 0.5f, -0.5f }), Vec3::Left(), Vec2::Zero() }, // Left
				Vertex{ Vec3({ -0.5f, 0.5f, 0.5f }), Vec3::Left(), Vec2({0.0f, 1.0f}) },
				Vertex{ Vec3({ -0.5f, -0.5f, 0.5f }), Vec3::Left(), Vec2({1.0f, 1.0f}) },
				Vertex{ Vec3({ -0.5f, -0.5f, -0.5f }), Vec3::Left(), Vec2({1.0f, 0.0f}) },
				Vertex{ Vec3({ 0.5f, 0.5f, -0.5f }), Vec3::Right(), Vec2::Zero() }, // Left
				Vertex{ Vec3({ 0.5f, 0.5f, 0.5f }), Vec3::Right(), Vec2({0.0f, 1.0f}) },
				Vertex{ Vec3({ 0.5f, -0.5f, 0.5f }), Vec3::Right(), Vec2({1.0f, 1.0f}) },
				Vertex{ Vec3({ 0.5f, -0.5f, -0.5f }), Vec3::Right(), Vec2({1.0f, 0.0f}) },
				Vertex{ Vec3({ -0.5f, -0.5f, -0.5f }), Vec3::Bottom(), Vec2::Zero() }, // Bottom
				Vertex{ Vec3({ 0.5f, -0.5f, -0.5f }), Vec3::Bottom(), Vec2({0.0f, 1.0f}) },
				Vertex{ Vec3({ 0.5f, -0.5f, 0.5f }), Vec3::Bottom(), Vec2({1.0f, 1.0f}) },
				Vertex{ Vec3({ -0.5f, -0.5f, 0.5f }), Vec3::Bottom(), Vec2({1.0f, 0.0f}) },
				Vertex{ Vec3({ -0.5f, 0.5f, -0.5f }), Vec3::Top(), Vec2::Zero() }, // Top
				Vertex{ Vec3({ 0.5f, 0.5f, -0.5f }), Vec3::Top(), Vec2({0.0f, 1.0f}) },
				Vertex{ Vec3({ 0.5f, 0.5f, 0.5f }), Vec3::Top(), Vec2({1.0f, 1.0f}) },
				Vertex{ Vec3({ -0.5f, 0.5f, 0.5f }), Vec3::Top(), Vec2({1.0f, 0.0f}) },
			},
			{
				0, 3, 2, 0, 2, 1,
				4, 5, 6, 4, 6, 7,
				8, 11, 10, 8, 10, 9,
				12, 13, 14, 12, 14, 15,
				18, 19, 16, 17, 18, 16,
				22, 21, 20, 23, 22, 20,
			}
		};

	}
}

#endif