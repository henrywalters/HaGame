#include "../../HaGame/HaGame.h"

struct Particle {
	Vec3 velocity;
	hagame::graphics::Color color;
	float created;
};

struct ParticleEmitter {
	float particleLifeDur = 3.0f;
	int particlesPerEmittion = 1;
	float particleSpeed = 1.0f;
	float delay = 1.0f;
	float timeSinceLastEmit = 0.0f;
	float gravity = 15.0f;
	hagame::graphics::Color particleColor = hagame::graphics::Color::blue();
	// hagame::graphics::Mesh* particleMesh;
	hagame::graphics::ShaderProgram* particleShader;
	float angularWidth = 0; // PI / 8;
	Queue<Particle> particles = Queue<Particle>();
	Array<float> data = Array<float>();
};

struct EmitterBuffers {
	unsigned int instanceVBO;
	unsigned int quadVAO, quadVBO;
};

class ParticleSystem : public hagame::ecs::System {
public:

	int MAX_PARTICLE_SIZE = 1000000;

	float startTime;

	hagame::math::Sample<long, 100> s1;
	hagame::math::Sample<long, 100> s2;
	hagame::math::Sample<long, 100> s3;

	Map<uint32_t, Ptr<EmitterBuffers>> emitterBuffers;

	String getSystemName() {
		return "ParticleSystem";
	}

	void onSystemStart() {
		
		startTime = game->secondsElapsed;

		s1.onFull = [this]() {
			std::cout << "Adding new: " << s1.average() << std::endl;
			s1.clear();
		};

		s2.onFull = [this]() {
			std::cout << "Purging: " << s2.average() << std::endl;
			s2.clear();
		};

		s3.onFull = [this]() {
			std::cout << "Updating: " << s3.average() << std::endl;
			s3.clear();
		};

		emitterBuffers = Map<uint32_t, Ptr<EmitterBuffers>>();

		forEach<ParticleEmitter>([this](ParticleEmitter* emitter, hagame::ecs::Entity* entity) {
			auto buffers = std::make_shared<EmitterBuffers>();

			emitter->particleShader->use();

			glGenBuffers(1, &buffers->instanceVBO);

			glCheckError();

			glBindBuffer(GL_ARRAY_BUFFER, buffers->instanceVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * MAX_PARTICLE_SIZE, NULL, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenVertexArrays(1, &buffers->quadVAO);
			glGenBuffers(1, &buffers->quadVBO);

			glBindVertexArray(buffers->quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, buffers->quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * hagame::graphics::QuadVertices.size(), &hagame::graphics::QuadVertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			emitterBuffers.insert(std::make_pair(entity->id, buffers));
		});
	}

	void onSystemUpdate(double dt) {

		glCheckError();

		forEach<ParticleEmitter>([dt, this](ParticleEmitter* emitter, hagame::ecs::Entity* entity) {

			hagame::utils::Timer t;

			emitter->timeSinceLastEmit += dt;
			if (emitter->timeSinceLastEmit >= emitter->delay) {
				emitter->timeSinceLastEmit -= emitter->delay;

				//entity->transform->setPosition(Vec3({(float)cos(game->secondsElapsed) ,0, (float)sin(game->secondsElapsed) * 1}));
				//emitter->direction = Quat(dt, Vec3::Top()).rotatePoint(emitter->direction);

				Vec3 entityTop = entity->transform->top();
				Vec3 axisA = Vec3({ 1, 0, 0 }).normalized();
				Vec3 axisB = entity->transform->top().cross(axisA);

				for (int i = 0; i < emitter->particlesPerEmittion; i++) {
					Particle particle;
					float r1 = game->random.real<float>(emitter->angularWidth / -2, emitter->angularWidth / 2);
					float r2 = game->random.real<float>(emitter->angularWidth / -2, emitter->angularWidth / 2);
					Vec2 r = Vec2({ r1, r2 });
					if (r.magnitude() >= emitter->angularWidth / 2) {
						r = r.normalized().prod(emitter->angularWidth / 2);
					}
					particle.velocity = (Quat(r[0], axisA) * Quat(r[1], axisB)).rotatePoint(entityTop) * emitter->particleSpeed;
					particle.color = emitter->particleColor;
					particle.created = game->secondsElapsed;
					emitter->particles.push_back(particle);

					emitter->data.insert(emitter->data.end(), {
						particle.velocity[0], particle.velocity[1], particle.velocity[2],
						1.0, 0.0, 1.0, 1.0,
						//particle.color[0], particle.color[0], particle.color[0], particle.color[0],
						(float) game->secondsElapsed,
					});
				}
			}

			s1.insert(t.elapsed());
			t.reset();

			bool removingDeadParticles = true;
			
			while (removingDeadParticles) {
				if (emitter->particles.size() == 0) {
					removingDeadParticles = false;
				}
				else {
					Particle head = emitter->particles.front();
					
					if (head.created - startTime > emitter->particleLifeDur) {
						// emitter->particles.pop_front();
						// delete head.get();
					}
					else {
						removingDeadParticles = false;
					}
				}
			}

			s2.insert(t.elapsed());
			t.reset();

			Array<float> data = Array<float>();
			/*
			for (auto particle : emitter->particles) {
				particle->aliveFor += dt;
				//particle->velocity = particle->velocity - Vec3({ 0, emitter->gravity, 0 }) * dt;
				//particle->position += particle->velocity * dt;
				particle->color[3] -= dt;

				for (int i = 0; i < 3; i++) {
					data.push_back(particle->position[i]);
				}

				for (int i = 0; i < 4; i++) {
					data.push_back(particle->color[i]);
				}
				
				//emitter->particleShader->setMVP(Mat4::Translation(particle->position) * Mat4::Scale(Vec3({ 0.1, 0.1, 0.1 })), viewMat, projMat);
				// emitter->particleShader->setVec4("color", particle->color);
				// emitter->particleMesh->draw(emitter->particleShader);
				
			}
			*/
			s3.insert(t.elapsed());
			t.reset();

			emitter->particleShader->use();
			emitter->particleShader->setFloat("gravity", emitter->gravity);
			emitter->particleShader->setFloat("current", game->secondsElapsed);
			emitter->particleShader->setMVP(entity->transform->getModelMatrix(), scene->viewMat, scene->projMat);

			/*std::cout << emitter->data.size() << std::endl;

			for (int i = 0; i < emitter->data.size() / 8; i++) {
				std::cout << "Elapsed: " << game->secondsElapsed << "\n";
				std::cout << "Created: " << emitter->data[i * 8 + 7] << "\n";
				std::cout << "T = " << (game->secondsElapsed - emitter->data[i * 8 + 7]) << "\n";
			}*/

			glBindBuffer(GL_ARRAY_BUFFER, emitterBuffers[entity->id]->instanceVBO);
			//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle) * emitter->particles.size(), &emitter->particles[0]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float) * emitter->data.size(), &emitter->data[0]);

			glCheckError();

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*) 0);
			glVertexAttribDivisor(1, 1);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*) (3 * sizeof(float)));
			glVertexAttribDivisor(2, 1);

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*) (7 * sizeof(float)));
			glVertexAttribDivisor(3, 1);
			
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(emitterBuffers[entity->id]->quadVAO);
			//glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 6 * emitter->particles.size());
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 6 * emitter->data.size());

			glCheckError();
		});
	}
};

class FPS : public hagame::Scene {
private:
	hagame::ecs::Entity* addGameObject(Vec3 pos, String mesh, hagame::graphics::Material mat, String tex, String shader) {
		auto entity = addEntity();
		entity->transform->position = pos;
		entity->transform->scale = BlockSize;
		auto renderer = entity->addComponent<hagame::graphics::MeshRenderer>();
		renderer->mesh = game->resources->getMesh(mesh);
		renderer->texture = game->resources->getTexture(tex);
		renderer->shader = game->resources->getShaderProgram(shader);
		renderer->opacityTexture = game->resources->getTexture("white");
		renderer->material = mat;
		// entity->addComponent<hagame::physics::Collider>()->dynamic = false;
		/*auto bbR = entity->addComponent<hagame::graphics::BoundingBoxRenderer>();
		bbR->boundingBox = renderer->mesh->getBoundingCube();
		bbR->shader = game->resources->getShaderProgram("color");
		bbR->color = RED;
		*/
		return entity;
		
	}
public:

	const hagame::graphics::Color RED = hagame::graphics::Color(217, 50, 83);
	const hagame::graphics::Color BLUE = hagame::graphics::Color(54, 134, 255);
	const hagame::graphics::Color GREEN = hagame::graphics::Color(0, 255, 0);

	hagame::ecs::Entity* player;
	hagame::ecs::Entity* obstacles;
	hagame::ecs::Entity* gun;
	hagame::ecs::Entity* camera;
	hagame::ecs::Entity* light;

	Ptr<hagame::graphics::PerspectiveCamera> pCamera = std::make_shared<hagame::graphics::PerspectiveCamera>();

	Vec3 BlockSize = Vec3(1.0);

	hagame::math::LineSegment* line = new hagame::math::LineSegment(Vec3({50, 0, 50}), Vec3({ 100, 5, 100 }));

	void setupCamera() {
		pCamera->aspectRatio = game->window->size[0] / game->window->size[1];
		//pCamera->transform->position = Vec3({ -5.0f, 5.0f, -5.0f });
		//->transform->rotation = Quat(0.0f, Vec3::Top());
		//pCamera->transform->lookAt(Vec3::Zero());
		//pCamera->transform->lookAt(Vec3({ 5.0f, 5.0f, 5.0f }));
	}

	void initializeResources() {
		game->resources->loadShaderProgram("material", "Shaders/material_vert.glsl", "Shaders/material_frag.glsl");
		game->resources->loadShaderProgram("color", "Shaders/color_vert.glsl", "Shaders/color_frag.glsl");
		game->resources->loadShaderProgram("texture", "Shaders/texture_vert.glsl", "Shaders/texture_frag.glsl");
		game->resources->loadShaderProgram("skybox", "Shaders/skybox_vert.glsl", "Shaders/skybox_frag.glsl");
		game->resources->loadShaderProgram("particle", "Shaders/particle_vert.glsl", "Shaders/particle_frag.glsl");
		game->resources->loadShaderProgram("text", "Shaders/text_vert.glsl", "Shaders/text_frag.glsl");
		game->resources->loadShaderProgram("text3d", "Shaders/text3d_vert.glsl", "Shaders/text_frag.glsl");
		game->resources->loadTexture("crate_specular", "Textures/crate_specular.jpg");
		game->resources->loadMesh("crystals", "Models/CrystalsFromAdrian.obj");
		game->resources->loadMesh("taurus", "Models/Sphere.obj");
		game->resources->loadMesh("player", "Models/Player.obj");
		game->resources->loadTexture("grass", "Textures/grass.png");
		game->resources->loadTexture("window", "Textures/window.png");
		game->resources->loadTexture("white", "Textures/white.jpg");
		game->resources->loadTexture("brick", "Textures/crate.jpg");
		game->resources->loadTexture("crate", "Textures/crate.jpg");
		game->resources->loadTexture("floor", "Textures/floor.jpg");
		game->resources->loadTexture("gun", "Textures/gun.jpg");
		game->resources->loadTexture("wood", "Textures/wood.jpg");
		game->resources->loadMesh("glock", "Models/Glock.obj");
		game->resources->loadMesh("cube", hagame::graphics::CubeMesh);
		game->resources->loadFile("map", "../Demos/FPS/level.txt");
		game->resources->loadMesh("kayak_rack", "Models/KayakRack.obj");
		game->resources->loadMesh("quad", hagame::graphics::QuadMesh);
		game->resources->loadTexture("wood_floor", "Textures/wood_floor.png");
		game->resources->loadFont("roboto", "Fonts/Roboto/Roboto-Regular.ttf", 32);

		String cubemapPaths[6] = {
			"Textures/skybox/right.jpg",
			"Textures/skybox/left.jpg",
			"Textures/skybox/top.jpg",
			"Textures/skybox/bottom.jpg",
			"Textures/skybox/front.jpg",
			"Textures/skybox/back.jpg",
		};

		game->resources->loadCubemap("skybox_1", cubemapPaths);

		game->resources->getFileSystem()->forEachFile("Textures/chainwall", [this](String path, String name) {
			game->resources->loadTexture("chainwall_" + name, path);
			std::cout << "Loaded texture: " << ("chainwall_" + name) << std::endl;
		});

		game->resources->getFileSystem()->forEachFile("Textures/stone", [this](String path, String name) {
			if (stringContains(name, '_')) {
				auto parts = stringSplit(name, '_');
				auto texName = "stone_" + parts[1];
				game->resources->loadTexture(texName, path);
				std::cout << "Loaded texture: " << texName << std::endl;
			}
			
		});
	}

	void addSystems() {
		addSystem<hagame::graphics::RenderSystem>();
		addSystem<hagame::physics::PhysicsSystem>();
		addSystem<hagame::physics::MovementSystem>();
		addSystem<hagame::physics::CollisionSystem>();
		addSystem<hagame::graphics::CameraSystem>();
		addSystem<hagame::graphics::LightingSystem>();
		//addSystem<ParticleSystem>();
	}

	void addGameObjects() {



		auto floor = addEntity();
		// floor->transform->rotation = Quat(0, Vec3::Top());
		auto fRenderer = floor->addComponent<hagame::graphics::MeshRenderer>();
		fRenderer->mesh = hagame::graphics::Plane<1000, 1000>(Vec2({ 1000, 1000 })).generateMesh().get();
		fRenderer->color = RED;
		fRenderer->shader = game->resources->getShaderProgram("texture");
		fRenderer->opacityTexture = game->resources->getTexture("chainwall_opacity");
		fRenderer->texture = game->resources->getTexture("wood_floor");
		

		player = addEntity();
		// player->addComponent<hagame::graphics::RigidBodyRenderer>()->shader = game->resources->getShaderProgram("color");
		player->addComponent<hagame::graphics::AxisRenderer>()->shader = game->resources->getShaderProgram("color");
		auto pColl = player->addComponent<hagame::physics::Collider>();
		player->transform->rotation = Quat(0, Vec3::Top());
		//player->transform->scale = Vec3({ 0.3f, 0.3f, 0.3f });
		auto rb = player->addComponent<hagame::physics::RigidBody>();
		rb->fixAxis[1] = true;
		auto movementController = player->addComponent<hagame::physics::SimpleMovement>();
		movementController->inputFn = [this]() {
			hagame::physics::SimpleMovementParameters params;
			params.movement = game->input.keyboardMouse.lAxis.normalized();
			params.running = game->input.keyboardMouse.isKeyDown(SDLK_LSHIFT);
			return params;
		};

		pColl->boundingCube = game->resources->getMesh("cube")->getBoundingCube();
		// pColl->aabb.center[1] += BlockSize[1] / 2.0f;
		//pColl->aabb.radius *= 0.6;
		std::cout << game->resources->getMesh("cube")->getBoundingCube().toString() << std::endl;
		std::cout << pColl->aabb.toString() << std::endl;
		pColl->display = true;
		pColl->dynamic = true;
		pColl->shader = game->resources->getShaderProgram("color");

		auto pBB = player->addComponent<hagame::graphics::BoundingBoxRenderer>();
		pBB->boundingBox = game->resources->getMesh("cube")->getBoundingCube();
		pBB->color = RED;
		pBB->shader = game->resources->getShaderProgram("color");
		

		auto pRenderer = player->addComponent<hagame::graphics::MeshRenderer>();
		pRenderer->color = BLUE;
		pRenderer->mesh = game->resources->getMesh("cube");
		pRenderer->texture = game->resources->getTexture("gun");
		pRenderer->material = hagame::graphics::Material::blackPlastic();
		pRenderer->shader = game->resources->getShaderProgram("texture");
		

		gun = addChild(player);
		gun->transform->rotation = Quat(0, Vec3::Top());
		auto gRenderer = gun->addComponent<hagame::graphics::MeshRenderer>();
		gRenderer->mesh = game->resources->getMesh("glock");
		gRenderer->color = BLUE;
		gRenderer->texture = game->resources->getTexture("gun");
		gRenderer->shader = game->resources->getShaderProgram("texture");
		gun->addComponent<hagame::graphics::AxisRenderer>()->shader = game->resources->getShaderProgram("color");
		// auto gRotate = gun1->addComponent<hagame::physics::RotationMovement>();

		camera = addChild(player);
		camera->transform->position = Vec3({ -1, 10, -1 });
		auto camComponent = camera->addComponent<hagame::graphics::CameraComponent>();
		camComponent->active = true;
		camComponent->camera = pCamera.get();

		auto camController = player->addComponent<hagame::graphics::FPSCameraController>();
		camController->camera = pCamera.get();
		camController->inputFn = [this]() {
			hagame::graphics::FPSCameraControllerParams params;
			params.look = game->input.keyboardMouse.rAxis;
			params.zoomed = game->input.keyboardMouse.mouse.right;
			return params;
		};

		//player->transform->move(Vec3::Top());


		auto pEmitter = addEntity();
		pEmitter->transform->setPosition(Vec3({ 1.5, 0, 0 }));
		auto emitter = pEmitter->addComponent<ParticleEmitter>();
		emitter->particleShader = game->resources->getShaderProgram("particle");
		emitter->particleColor = hagame::graphics::Color::green();
		// pEmitter->transform->rotate(Quat(-PI / 8, Vec3::Face()));
		
		auto pEmitter2 = addEntity();
		pEmitter2->transform->setPosition(Vec3({ 3, 0, 0 }));
		auto emitter2 = pEmitter2->addComponent<ParticleEmitter>();
		emitter2->particleShader = game->resources->getShaderProgram("particle");
		emitter2->particleColor = hagame::graphics::Color::red();
		emitter2->particleSpeed = 25.0f;
		//emitter2->gravity = 10.0f;
		emitter2->gravity = 0.1;
		emitter2->particleLifeDur = 5.0f;
		emitter2->angularWidth = PI / 16;
		/*
		
		auto pEmitter3 = addEntity();
		pEmitter3->transform->setPosition(Vec3({ 4.5, 0, 0 }));
		auto emitter3 = pEmitter3->addComponent<ParticleEmitter>();
		emitter3->particleShader = game->resources->getShaderProgram("particle");
		emitter3->particleColor = hagame::graphics::Color::green();
		pEmitter3->transform->rotate(Quat(PI / 8, Vec3::Face()));
		*/
		gun->transform->setPosition(player->transform->face() * 2 + Vec3({ -0.6, -1, 0 }));

		/*auto pRenderer = player->addComponent<hagame::graphics::MeshRenderer>();
		pRenderer->mesh = game->resources->getMesh("cube");
		pRenderer->color = BLUE;
		pRenderer->shader = game->resources->getShaderProgram("color");
		*/
		obstacles = addEntity();

		int yIndex = 0;

		Array<hagame::graphics::ConcatMeshDefinition> wallMeshes = Array<hagame::graphics::ConcatMeshDefinition>();
		Array<Vec3> wallColliders = Array<Vec3>();

		for (String line : game->resources->getFile("map")->readLines()) {
			for (int i = 0; i < line.size(); i++) {

				Vec3 pos = Vec3({ (float)i * BlockSize[0], 0, (float)yIndex * BlockSize[2] }).prod(BlockSize);

				if (line[i] == '#') {
					hagame::graphics::ConcatMeshDefinition meshDef;
					Vec3 wallPos = Vec3({ (float)i, 0, (float)yIndex });
					meshDef.mesh = game->resources->getMesh("cube")->getDefinition();
					meshDef.model = Mat4::Translation(wallPos);
					wallMeshes.push_back(meshDef);
					wallColliders.push_back(pos);
				}

				if (line[i] == 'K') {
					addGameObject(
						pos,
						"kayak_rack",
						hagame::graphics::Material::gold(),
						"wood",
						"texture"
					);
				}

				if (line[i] == 'G') {
					addGameObject(
						pos + Vec3({ 0, BlockSize[1], 0 }),
						"glock",
						hagame::graphics::Material::obsidian(),
						"gun",
						"texture"
					)->addComponent<hagame::physics::RotationMovement>();
				}

				if (line[i] == 'T') {
					addGameObject(
						pos + Vec3({ 0, BlockSize[1], 0 }),
						"taurus",
						hagame::graphics::Material::emerald(),
						"white",
						"texture"
					)->addComponent<hagame::physics::RotationMovement>()->axis = Vec3::Right();
				}

				if (line[i] == 'C') {
					auto cube = addGameObject(
						pos + Vec3({ 0, BlockSize[1], 0 }),
						"cube",
						hagame::graphics::Material::whiteRubber(),
						"crate",
						"texture"
					);

					cube->getComponent<hagame::graphics::MeshRenderer>()->specularTexture = game->resources->getTexture("crate_specular");

					cube->addComponent<hagame::physics::RotationMovement>()
						->axis = Vec3::Identity();

					cube->transform->scale = Vec3({ 0.3, 0.3, 0.3 });
				}

				if (line[i] == 'W') {
					addGameObject(
						pos + Vec3({ 0, BlockSize[1], 0 }),
						"quad",
						hagame::graphics::Material::ruby(),
						"window",
						"texture"
					);
				}

				if (line[i] == 'g') {
					addGameObject(
						pos,
						"quad",
						hagame::graphics::Material::ruby(),
						"grass",
						"texture"
					);
				}

				if (line[i] == 'P') {
					// player->transform->setPosition(Vec3({ pos[0], BlockSize[1] / 2.0f, pos[2] }));
				}
			}
			yIndex++;
			
		}


		light = addEntity();
		light->transform->position = Vec3({ 10.0, 10.0, 10.0 });
		auto lightC = light->addComponent<hagame::graphics::PointLightComponent>();
		lightC->light.ambient = Vec3({ 0.4, 0.4, 0.4 });
		lightC->light.diffuse = Vec3({ 0.5, 0.5, 0.5 });
		lightC->light.specular = Vec3({ 0.7, 0.7, 0.7 });
		auto lightR = light->addComponent<hagame::graphics::MeshRenderer>();
		lightR->material = hagame::graphics::Material::whitePlastic();
		lightR->texture = game->resources->getTexture("white");
		lightR->mesh = game->resources->getMesh("cube");
		lightR->shader = game->resources->getShaderProgram("texture");


		game->resources->loadMesh("walls", wallMeshes);

		auto walls = addGameObject(
			Vec3::Zero(),
			"walls",
			hagame::graphics::Material::silver(),
			"stone_basecolor",
			"texture"
		);

		for (auto wallCollider : wallColliders) {
			auto collider = addChild(walls);
			collider->transform->setPosition(wallCollider);
			auto colliderC = collider->addComponent<hagame::physics::Collider>();
			colliderC->boundingCube = game->resources->getMesh("cube")->getBoundingCube();
			colliderC->display = false;
			colliderC->dynamic = false;
			colliderC->shader = game->resources->getShaderProgram("color");

			/*auto bbR = collider->addComponent<hagame::graphics::BoundingBoxRenderer>();
			bbR->boundingBox = game->resources->getMesh("cube")->getBoundingCube();
			bbR->color = GREEN;
			bbR->shader = game->resources->getShaderProgram("color");
			*/
		}
		walls->transform->move(BlockSize.prod(Vec3::Top()) * 0.5);
		// walls->transform->setScale(BlockSize);
		
		auto renderer = walls->getComponent<hagame::graphics::MeshRenderer>();
		// walls->specularTexture = game->resources->getTexture("crate_specular");
		renderer->normalMap = game->resources->getTexture("stone_normal");

		/*auto skybox = addEntity()
			->addComponent<hagame::graphics::SkyboxRenderer>();

		skybox->cubemap = game->resources->getCubemap("skybox_1");
		skybox->shader = game->resources->getShaderProgram("skybox");
		*/

		auto textContainer = addEntity();
		textContainer->addComponent<hagame::physics::RotationMovement>();
		auto text = textContainer->addComponent<hagame::graphics::Text3dRenderer>();
		text->message = "Hello World";
		text->shader = game->resources->getShaderProgram("text3d");
		text->font = game->resources->getFont("roboto");
		text->maxLength = 0.5f;
	}

	void onSceneActivate() {
		std::cout << "ACtivating FPS";
		initializeResources();
		setupCamera();
		addSystems();
		addGameObjects();

		game->window->clearColor = hagame::graphics::Color(5, 5, 5);
	}

	void onSceneUpdate(double dt) {

		// activeCamera->transform->position = player->transform->position + Vec3({ 0, 2.0, 0 });
		if (game->input.keyboardMouse.aPressed) {
			game->input.keyboardMouse.toggleMouseCapture();
		}
		game->resources->getShaderProgram("text3d")->use();
		game->resources->getShaderProgram("text3d")->setMVP(Mat4::Translation(Vec3({ BlockSize[2] - 0.1f, 0, 0 })) * Mat4::Scale(Vec3(game->resources->getFont("roboto")->getScale())), viewMat, projMat);
		//game->resources->getShaderProgram("text3d")->setMat4("projection", Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1));
		//auto message = hagame::graphics::Text(game->resources->getFont("roboto"), "fps: " + std::to_string(game->fps / 1000000), Vec2({30, 30}));
		//message.message = "The quick brown fox jumps over the lazy dog. 1234567890";
		//message.draw(game->resources->getShaderProgram("text3d"));
		/*
		game->resources->getShaderProgram("text")->use();
		game->resources->getShaderProgram("text")->setMat4("projection", Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1));
		message = hagame::graphics::Text(game->resources->getFont("roboto"), "fps: " + std::to_string(game->fps / 1000000), Vec2({ 15, game->window->size[1] - 100 }), BLUE);
		message.message = "DT: " + std::to_string(dt);
		message.draw(game->resources->getShaderProgram("text"));

		message = hagame::graphics::Text(game->resources->getFont("roboto"), "Player: " + player->transform->position.toString(), Vec2({ 15, game->window->size[1] - 50 }) , BLUE);
		message.draw(game->resources->getShaderProgram("text"));
		*/
		game->resources->getShaderProgram("text")->use();
		game->resources->getShaderProgram("text")->setMat4("projection", Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1));

		hagame::graphics::drawText(
			game->resources->getShaderProgram("text"),
			game->resources->getFont("roboto"),
			"Player: " + player->transform->position.toString(),
			BLUE,
			Vec3({ 15, game->window->size[1] - 25, 0 }),
			0.5f
		);


		light->transform->position = Vec3({ 30.0f, (float) sin(game->secondsElapsed / 100.0f) * 10.0f, (float) cos(game->secondsElapsed / 100.0f) * 10.0f });

		auto lineRenderer = hagame::graphics::Line(line->a, line->b, BLUE);
		
		auto viewMat = pCamera->getViewMatrix(camera->transform.get());
		auto projMat = pCamera->getProjMatrix();

		game->resources->getShaderProgram("color")->setMVP(Mat4::Identity(), viewMat, projMat);
		game->resources->getShaderProgram("color")->setVec4("color", BLUE);
		lineRenderer.draw(game->resources->getShaderProgram("color"));

		float t;
		auto nearestPoint = line->closestPoint(player->transform->position, t);

		game->resources->getShaderProgram("color")->setMVP(Mat4::Translation(nearestPoint) * Mat4::Scale(Vec3({ 0.2, 0.2, 0.2 })), viewMat, projMat);
		game->resources->getMesh("cube")->draw(game->resources->getShaderProgram("color"));

		// camera->transform->lookAt(player->transform->position);

		// obstacles->transform->move(Vec3({ 1, 0, 1 }) * dt);
		// activeCamera->transform->lookAt(player->transform->position + player->transform->face());
	}
};