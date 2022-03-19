#ifndef QUAD_RENDERER
#define QUAD_RENDERER

#include "./../Quad.hpp"
#include "./../Color.h"
#include "./../ShaderProgram.h"
#include "./../../UI/Components/UIComponent.h"

namespace hagame {
	namespace graphics {
		struct QuadRenderer : public hagame::ui::UIComponent {
			Ptr<Quad> quad;
			Color color;
			ShaderProgram* shader;

			QuadRenderer(Vec2 size) :
				quad(std::make_shared<Quad>(size)) {}

			Rect getBoundingBox() {
				return Rect(Vec2::Zero(), Vec2(quad->getWidth(), quad->getHeight()));
			}
		};
	}
}

#endif