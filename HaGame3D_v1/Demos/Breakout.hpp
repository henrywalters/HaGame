#ifndef BREAKOUT
#define BREAKOUT

#define DEBUG_MODE true

#include "../Input/Device.h"
#include "../Input/Devices/KeyboardMouse.h"
#include "../Graphics/Window.h"
#include "../Graphics/Monitors.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Sprite.h"
#include "../Graphics/Routines.h"
#include "../Core/ResourceManager.h"
#include "../Core/Game.h"

namespace hagame {

	class Breakout : public hagame::Game {
	public:

		enum class BrickType {
			Empty,
			Normal,
			Solid,
		};

		struct Brick {
			Cube pos;
			BrickType type;
			bool alive;
		};

		const float initBallSpeed = 200.0f;
		const float paddleSpeed = 400.0f;

		const int rows = 100;
		const int cols = 100;

		float ballSpeed = initBallSpeed;
		
		Vec2 cellSize;
		Vec3 paddleSize;
		Vec3 ballSize;
		Vec3 brickSize;

		Vec3 screenSize;

		GLuint quadVAO;

		Ptr<input::Device> controller;
		Ptr<graphics::Window> window;
		Ptr<graphics::Camera> camera;

		Ptr<ResourceManager> textures;
		Ptr<ResourceManager> shaders;
		Ptr<ResourceManager> levels;

		Ptr<graphics::Sprite> sprite;

		Ptr<graphics::ShaderProgram> spriteShader;
		Ptr <graphics::ShaderProgram> colorShader;

		Array<Brick> bricks;
		Cube player;
		Cube ball;

		Breakout(Vec2 size) : screenSize(Vec3({ size[0], size[1] })) {
			textures = std::make_shared<ResourceManager>("../../../HaGame3D_v1/Assets/Textures");
			shaders = std::make_shared<ResourceManager>("../../../HaGame3D_v1/Assets/Shaders");
			levels = std::make_shared<ResourceManager>("../../../HaGame3D_v1/Demos/levels");

			cellSize = Vec2({ screenSize[0] / cols, screenSize[1] / rows });
			paddleSize = Vec3({ cellSize[0] * 15, cellSize[1] * 2});
			ballSize = Vec3({ cellSize[0] * 3, cellSize[1] * 3 });
			brickSize = Vec3({ cellSize[0] * 5, cellSize[1] * 3 });
		}

		void onGameStart() {

			// Boiler plate code
			std::cout << "Welcome to breakout\n";
			SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);

			controller = std::make_shared<input::devices::KeyboardMouse>();
			controller->pollDevice();

			window = std::make_shared<graphics::Window>(graphics::Window::ForMonitor(graphics::MonitorManager::GetMonitor(0)));
			window->create();

			quadVAO = graphics::gl::loadVAO<float, 4>(graphics::QuadVertices);

			camera = std::make_shared<graphics::Camera>(Vec3({ screenSize[0], screenSize[1] }));

			// Load texture assets to be used in game
			textures->loadTexture("brick", "blue.jpg");
			textures->loadTexture("solid_brick", "grey.png");
			textures->loadTexture("ball", "natalie.jpg");
			textures->loadTexture("paddle", "paddle.png");

			// Load shaders
			spriteShader = shaders->loadShaderProgram("sprite", "sprite_vert.glsl", "sprite_frag.glsl");
			colorShader = shaders->loadShaderProgram("color", "color_vert.glsl", "color_frag.glsl");

			// Create objects
			player = Cube((screenSize * 0.5f) - (paddleSize * 0.5f) + Vec3({ 0.0f, screenSize[1] * 0.4f }), paddleSize);
			ball = Cube(screenSize * 0.5f, ballSize);
			sprite = std::make_shared<hagame::graphics::Sprite>(textures->getTexture("ball"), Rect({ Vec2({50.0f, 50.0f}), Vec2({20.0f, 20.0f}) }), 0.0f, hagame::graphics::Color::white(), -1.0f);
			bricks = {};

			auto levelLines = levels->loadFile("level1", "level1")->readLines();

			levels->getFile("level1")->append("Hello World");

			for (int i = 0; i < levelLines.size(); i++) {
				for (int j = 0; j < 20; j++) {
					Cube cube = Cube(Vec3({ j * brickSize[0], i * brickSize[1] }), brickSize);
					BrickType type = levelLines[i][j] == '2' ? BrickType::Solid : BrickType::Normal;

					bricks.push_back(Brick{ cube, type, true });
				}
			}

		}

		void onGameUpdate(double dt) {

			if (controller->homePressed) {
				running = false;
			}

			controller->pollDevice();
			window->clear();
			glEnable(GL_DEPTH_TEST);

			// Game code here
			auto shader = shaders->getShaderProgram("sprite");
			shader->use();
			shader->setMat4("projection", camera->getOrthographicMatrix());
			shader->setMat4("view", camera->getViewMatrix());

			handlePaddle(dt);
			handleBall(dt);

			graphics::drawTexture(quadVAO, spriteShader, textures->getTexture("ball"), ball, graphics::Color::white());
			graphics::drawTexture(quadVAO, spriteShader, textures->getTexture("paddle"), player, graphics::Color::white());

			for (auto brick : bricks) {
				if (brick.alive) {
					if (brick.type == BrickType::Normal) {
						graphics::drawTexture(quadVAO, spriteShader, textures->getTexture("brick"), brick.pos, graphics::Color::white());
					}
					else {
						graphics::drawTexture(quadVAO, spriteShader, textures->getTexture("solid_brick"), brick.pos, graphics::Color::white());
					}
				}
			}

			window->render();
		}

		void onGameEnd() {
			std::cout << "Thanks for playing :)\n";
		}

		void handleBall(double dt) {

		}

		void handlePaddle(double dt) {
			float vel = paddleSpeed * controller->lAxis[0] * dt;
			if (vel < 0) {
				float newPos = player.pos[0] + vel;
				if (newPos <= 0) {
					vel -= newPos;
				}
			}

			if (vel > 0) {
				float newPos = player.pos[0] + player.size[0] + vel;
				if (newPos >= screenSize[1]) {
					vel += screenSize[1] - newPos;
				}
			}

			player.pos += Vec3({ vel, 0.0f });
		}
	};

}

#endif
