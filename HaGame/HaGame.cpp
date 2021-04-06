// HaGame3D_v1.cpp : Defines the entry point for the application.
//
#define SDL_MAIN_HANDLED // insert this
#include "Demos/ECS.hpp"


int main()
{


	auto ecs = hagame::ECSDemo();
	ecs.run();

	//breakout.run();

	/*
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);

	auto monitors = hagame::graphics::MonitorManager::GetAvailableMonitorCount();

	auto window = std::make_shared<hagame::graphics::Window>(Vec2({ 800, 800 }), "Demo");

	auto resources = hagame::ResourceManager("../../../HaGame3D_v1/Assets");

	window->create();

	auto gamepad = new hagame::input::devices::KeyboardMouse();
	bool running = true;

	auto camera = hagame::graphics::Camera(Vec3({ 800, 800 }));

	float dt = 0;
	float t = 0;

	window->clearColor = hagame::graphics::Color::blue();

	// Sprite Test

	resources.loadTexture("nat", "Textures/natalie.jpg");
	resources.loadTexture("cat", "Textures/kitty.jpg");
	resources.loadTexture("fire", "Textures/fireball.png");

	auto spriteShader = resources.loadShaderProgram("sprite", "Shaders/sprite_vert.glsl", "Shaders/sprite_frag.glsl");
	auto particleShader = resources.loadShaderProgram("particle", "Shaders/particle_vert.glsl", "Shaders/particle_frag.glsl");

	spriteShader->use();
	spriteShader->setMat4("projection", camera.getOrthographicMatrix());

	particleShader->use();
	particleShader->setMat4("projection", camera.getOrthographicMatrix());

	auto renderer = std::make_shared<hagame::graphics::SpriteRenderer>(resources.getShaderProgram("sprite"));
	auto particleRenderer = std::make_shared<hagame::graphics::SpriteRenderer>(resources.getShaderProgram("sprite"));

	auto player = hagame::graphics::Sprite(resources.getTexture("nat"), Rect(Vec2({ 100, 100 }), Vec2({ 200, 200 })), 45.0f, hagame::graphics::Color::white(), 0.0f);

	using Sprite = hagame::graphics::Sprite;
	using Particle = hagame::graphics::Particle;

	int max_particles = 50;
	Array<Sprite> enemies = Array<Sprite>();
	Queue<Sprite> particles = Queue<Sprite>();

	for (int i = 0; i < 1000; i++) {

		enemies.push_back(hagame::graphics::Sprite(resources.getTexture("cat"), Rect({ Vec2({i * 50.0f, i * 50.0f}), Vec2({20.0f, 20.0f}) }), 0.0f, hagame::graphics::Color::white(), -1.0f));
	}

	player.origin = player.rect.size * 0.5f;

	while (running) {

		glEnable(GL_DEPTH_TEST);
		window->clear();

		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}

		auto t1 = hagame::utils::Clock::Now();

		gamepad->pollDevice();

		if (gamepad->homePressed) {
			// running = false;
		}

		float speed = 200;

		//if (gamepad->lAxis.magnitude() > 0.2) {
		//	player.rect.pos += gamepad->lAxis * dt * speed;
		//}

		player.rect.pos = gamepad->getMousePos() - player.rect.size * 0.5;

		Sprite particle = Sprite(resources.getTexture("nat"), Rect({ player.rect.pos, Vec2({200.0f, 200.0f}) }), player.rotation, hagame::graphics::Color::white(), max_particles - particles.size());
		particles.push_back(particle);

		if (particles.size() > max_particles) {
			particles.pop_front();
		}

		for (auto &particle : particles) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			hagame::graphics::drawSprite(window, renderer, particle);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		

		player.rotation = getAngle<float>((gamepad->mousePos - (player.rect.pos + player.origin))) * RAD_TO_DEG;

		t += dt;

		hagame::graphics::drawSprite(window, renderer, player);

		for (auto enemy : enemies) {
			hagame::graphics::drawSprite(window, renderer, enemy);
		}

		window->render();

		dt = (hagame::utils::Clock::Now() - t1) / 1000000.0f;
		auto FPS = 1 / (dt );

		std::cout << FPS << std::endl;

	}
	return 0;
	*/
}
