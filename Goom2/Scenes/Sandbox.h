#ifndef GOOM_SANDBOX_H
#define GOOM_SANDBOX_H

#include "../../HaGame/HaGame.h"

class Sandbox : public hagame::Scene {

	Ptr<hagame::ecs::Entity> player;

	void onSceneActivate() {
		player = addEntity();
		auto pSpriteRenderer = player->addComponent<hagame::graphics::SpriteRenderer>();
		pSpriteRenderer->sprite = hagame::graphics::Sprite(game->resources->getTexture("kitty"), Rect(Vec2::Zero(), Vec2(100.0f)));
		pSpriteRenderer->shader = game->resources->getShaderProgram("sprite");
		pSpriteRenderer->projection = Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1);
	}

	void onSceneUpdate(double dt) {
		player->transform->position = Vec3({ game->input.keyboardMouse.mouse.position[0], game->window->size[1] - game->input.keyboardMouse.mouse.position[1], 0 });
	}
};

#endif