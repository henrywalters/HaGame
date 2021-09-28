#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "../Sprite.h"
#include "../../Core/ECS/Component.h"

namespace hagame {
	namespace graphics  {
		struct SpriteRenderer : public hagame::ecs::Component {
			Ptr<Sprite> sprite;
			ShaderProgram* shader;
			String getName() {
				return "Sprite Renderer";
			}

			SpriteRenderer() {
				sprite = std::make_shared<Sprite>();
				shader = NULL;
			}
		};
	}
}

#endif