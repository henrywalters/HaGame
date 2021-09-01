#ifndef PLAYER_RENDERER_H
#define PLAYER_RENDERER_H

#include "./../../HaGame/HaGame.h"

struct PlayerRenderer {
	Ptr<hagame::utils::StateManager<hagame::graphics::AnimatedSprite>> body;
	Ptr<hagame::utils::StateManager<hagame::graphics::AnimatedSprite>> feet;
	hagame::graphics::ShaderProgram* shader;
	Vec2 size = Vec2(50.0f);

	PlayerRenderer() {
		body = std::make_shared<hagame::utils::StateManager<hagame::graphics::AnimatedSprite>>();
		feet = std::make_shared<hagame::utils::StateManager<hagame::graphics::AnimatedSprite>>();
	}
};

#endif
