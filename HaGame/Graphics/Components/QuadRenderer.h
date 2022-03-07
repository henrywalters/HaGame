#ifndef QUAD_RENDERER
#define QUAD_RENDERER

#include "./../Quad.hpp"
#include "./../Color.h"
#include "./../ShaderProgram.h"

namespace hagame {
	namespace graphics {
		struct QuadRenderer {
			Ptr<Quad> quad;
			Color color;
			ShaderProgram* shader;

			QuadRenderer(Vec2 size) :
				quad(std::make_shared<Quad>(size)) {}
		};
	}
}

#endif