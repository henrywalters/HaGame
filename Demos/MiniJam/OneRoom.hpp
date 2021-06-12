#include "../../HaGame/HaGame.h"
#include "./ResourceSystem.h"
#include "./DrawingSystem.h"

class MiniJamOneRoom : public hagame::Scene {
public:

	hagame::graphics::Sprite player;

	MiniJamOneRoom() {
		
	}

	void onSceneBeforeActivate() {
		std::cout << "MiniJamOneRoom" << std::endl;
		addSystem<ResourceSystem>();
		addSystem<DrawingSystem>();
	}

	void onSceneAfterActivate() {
		player = hagame::graphics::Sprite(game->resources->getTexture("kitty"), Rect(Vec2({ 100, 100 }), Vec2({ 100, 100 })));
	}

	void onSceneUpdate(double dt) {

		player.rect.pos += game->input.keyboardMouse.lAxis * dt * 100;

		auto orth = Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1);
		auto model = Mat4::Translation(player.rect.pos.resize<3>()) * Mat4::Scale(player.rect.size.resize<3>());

		auto shader = game->resources->getShaderProgram("sprite");
		shader->use();
		shader->setMat4("projection", orth);
		shader->setMat4("model", model);
		player.draw(game->resources->getShaderProgram("sprite"));

		game->resources->getShaderProgram("text")->use();
		game->resources->getShaderProgram("text")->setMat4("projection", orth);
		hagame::graphics::drawText(
			game->resources->getShaderProgram("text"),
			game->resources->getFont("roboto"),
			"Cursor: " + game->input.keyboardMouse.mouse.position.toString(),
			hagame::graphics::Color::blue(),
			Vec3({ 15, game->window->size[1] - 25, 0 }),
			0.5f
		);
	}
};