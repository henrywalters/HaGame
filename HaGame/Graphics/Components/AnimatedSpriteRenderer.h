#ifndef ANIMATED_SPRITE_RENDERER_H
#define ANIMATED_SPRITE_RENDERER_H

#include "../../Utils/StateManager.h"
#include "../AnimatedSprite.h"

namespace hagame {
	namespace graphics {
		struct AnimatedSpriteRenderer {
			Ptr<utils::StateManager<AnimatedSprite>> sprites;
			ShaderProgram* shader;

			AnimatedSpriteRenderer():
				sprites(std::make_shared<utils::StateManager<AnimatedSprite>>())
			{
			}

			~AnimatedSpriteRenderer() {
				std::cout << "Killed sprite renderer\n";
			}
		};
	}
}

#endif