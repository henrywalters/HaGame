#include "../../HaGame/HaGame.h"
#include "./ResourceSystem.h"
#include "./DrawingSystem.h"
#include "./MovementSystem.h"

class MiniJamOneRoom : public hagame::Scene {
public:

	Mat4 orth;

	MiniJamOneRoom() {
		
	}

	void onSceneBeforeActivate() {
		std::cout << "MiniJamOneRoom" << std::endl;
		orth = Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1);
		addSystem<ResourceSystem>();
		addSystem<DrawingSystem>();
		addSystem<MovementSystem>();
		addSystem<hagame::graphics::RenderSystem>();
	}

	void onSceneAfterActivate() {
		auto player = addEntity();
		player->addComponent<PlayerMovement>();
		auto playerSprite = player->addComponent<hagame::graphics::SpriteRenderer>();
		playerSprite->sprite = hagame::graphics::Sprite(game->resources->getTexture("circle"), Rect(Vec2::Zero(), Vec2(25.0f)));
		playerSprite->shader = game->resources->getShaderProgram("sprite");
		playerSprite->projection = orth;
		player->transform->move(Vec3({ game->window->size[0] / 2, game->window->size[1] / 2, 0 }));

	}

	void onSceneUpdate(double dt) {
		Vec2 mousePos = game->input.keyboardMouse.mouse.position;
		mousePos[1] = game->window->size[1] - mousePos[1];

		game->resources->getShaderProgram("text")->use();
		game->resources->getShaderProgram("text")->setMat4("projection", orth);
		hagame::graphics::drawText(
			game->resources->getShaderProgram("text"),
			game->resources->getFont("roboto"),
			"Cursor: " + mousePos.toString(),
			hagame::graphics::Color::blue(),
			Vec3({ 15, game->window->size[1] - 25, 0 }),
			0.5f
		);

		auto drawing = getSystem<DrawingSystem>();
		auto playerCircle = Circle(mousePos, 12.5);

		for (auto stroke : drawing->strokes) {
			if (stroke->boundingBox.contains(mousePos)) {
				for (auto point : stroke->points) {
					if (playerCircle.contains(point)) {
						hagame::graphics::drawText(
							game->resources->getShaderProgram("text"),
							game->resources->getFont("roboto"),
							"Colliding at " + point.toString(),
							hagame::graphics::Color::blue(),
							Vec3({ 15, game->window->size[1] - 50, 0 }),
							0.5f
						);
						break;
					}
				}
			}
		}
	}
};