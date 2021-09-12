#ifndef PLAYER_SYSTEM_H
#define PLAYER_SYSTEM_H

#include "../../HaGame/HaGame.h"
#include "./../Components/PlayerRenderer.h"
#include "./../Components/PlayerController.h"
#include "./../Components/Bullet.h"
#include "./../Components/Projectile.h"
#include "./../Components/Inventory.h"
#include "./../Systems/WeaponSystem.h"
#include "./../Components/Health.h"
#include "./../Components/Actor.h"
#include "./../Components/Enemies/Turret.h"

const Array<String> map = {
	"#############################",
	"#############################",
	"#############################",
	"#############################",
	"#############################",
	"#############################",
	"#############################",
	"                            ",
	"     T                 #    ",
	"              T        #    ",
	"         T         T        ",
	"                       #    ",
	"     T         P       #    ",
	"           T                ",
	"#################  ##########",
	"#################  ##########",
	"#################  ##########",
	"###########        ##########",
	"###########      T ##########",
	"##########         ##########",
	"#############################"
};

const float CUBE_SIZE = 75.0f;

class PlayerSystem : public hagame::ecs::System {
public:

	Ptr<hagame::ecs::Entity> player;
	PlayerRenderer* playerRenderer;
	PlayerController* playerController;
	hagame::physics::Collider* playerCollider;
	Inventory* inventory;
	Health* pHealth;

	hagame::math::Sample<double, 10000> circleTest = hagame::math::Sample<double, 10000>([this]() {
		std::cout << "Circle collision time: " << circleTest.average() << " std = " << circleTest.stddev() << "\n";
		circleTest.clear();
	});
	hagame::math::Sample<double, 10000> boxTest = hagame::math::Sample<double, 10000>([this]() {
		std::cout << "Box collision time: " << boxTest.average() << " std = " << boxTest.stddev() << "\n";
		boxTest.clear();
	});
	hagame::utils::Timer timer;

	String getSystemName() {
		return "PlayerSystem";
	}

	void addWall(Vec2 pos, Vec2 size) {
		auto target = scene->addEntity();
		auto targetRenderer = target->addComponent<hagame::graphics::SpriteRenderer>();
		//targetRenderer->shader = game->resources->getShaderProgram("sprite");
		//targetRenderer->sprite = std::make_shared<hagame::graphics::Sprite>(
		//	game->resources->getTexture("stones"),
		//	Rect(size * 0.5, size)
		//	);
		target->transform->setPosition(pos.resize<3>() + Vec3({0, 0, 0.0f}));
		auto targetCollider = target->addComponent<hagame::physics::Collider>();
		targetCollider->dynamic = false;
		targetCollider->boundingCube = Cube(0.0f, size.resize<3>());
	}

	void addTarget(Vec2 pos, float radius) {
		auto target = scene->addEntity();
		target->addComponent<Health>();
		auto targetRenderer = target->addComponent<hagame::graphics::SpriteRenderer>();
		//targetRenderer->shader = game->resources->getShaderProgram("sprite");
		//targetRenderer->sprite = std::make_shared<hagame::graphics::Sprite>(
		//	game->resources->getTexture("target"),
		//	Rect(Vec2::Zero(), Vec2(radius * 2.0))
		//	);
		
		auto targetCollider = target->addComponent<hagame::physics::Collider>();
		targetCollider->dynamic = false;
		targetCollider->type = hagame::physics::ColliderType::SphereCollider;
		targetCollider->boundingSphere = Sphere(Vec3::Zero(), radius);
		targetCollider->ignoreTags.push_back("player");

		auto targetHealth = scene->addChild(target);
		targetHealth->transform->lockRotation = true;
		targetHealth->transform->setPosition(Vec3({ 0, 50, 0 }));
		auto targetHealthRenderer = targetHealth->addComponent<hagame::graphics::BoxRenderer>();
		targetHealthRenderer->box = Rect(Vec2({ 0, -10 }), Vec2({ 40, 2 }));
		targetHealthRenderer->borderColor = hagame::graphics::Color::red();
		targetHealthRenderer->fillColor = hagame::graphics::Color::green();
		targetHealthRenderer->shader = game->resources->getShaderProgram("color");
		
		target->transform->setPosition(pos.resize<3>() + Vec3({ 0, 0, 0.0f }));
	}

	void addTurret(Vec2 pos) {
		auto turret = scene->addEntity();
		auto actor = turret->addComponent<Actor>();
		turret->transform->setPosition(pos.resize<3>());
		actor->game = game;
		actor->def = TURRET;
		turret->addTag(actor->def.tagName);
		actor->state = "SEARCH";
		actor->weapon = std::make_shared<Weapon>(PLASMA_RIFLE_DEF);

		auto targetCollider =turret->addComponent<hagame::physics::Collider>();
		targetCollider->dynamic = false;
		targetCollider->type = hagame::physics::ColliderType::SphereCollider;
		targetCollider->boundingSphere = Sphere(Vec3::Zero(), 25.0f);
		targetCollider->ignoreTags.push_back("player");

		auto turretRenderer = turret->addComponent<hagame::graphics::SpriteRenderer>();
		//turretRenderer->shader = game->resources->getShaderProgram("sprite");
		//turretRenderer->sprite = std::make_shared<hagame::graphics::Sprite>(
		//	game->resources->getTexture("turret"),
		//	Rect(Vec2::Zero(), Vec2(50.0f))
		//);
	}

	void onSystemInit() {

		game->input.keyboardMouse.hideCursor();

		std::cout << "Initializing player system\n";


		player = scene->addEntity();
		player->addTag("player");
		// pHealth = player->addComponent<Health>();
		playerController = player->addComponent<PlayerController>();
		inventory = player->addComponent<Inventory>();
		
		/*auto healthBar = scene->addChild(player);
		healthBar->transform->lockRotation = true;
		healthBar->transform->setPosition(Vec3({ 0, 50, 0 }));
		auto healthRenderer = healthBar->addComponent<hagame::graphics::BoxRenderer>();
		healthRenderer->box = Rect(Vec2({ 0, -10 }), Vec2({ 50, 3 }));
		healthRenderer->borderColor = hagame::graphics::Color::red();
		healthRenderer->fillColor = hagame::graphics::Color::green();
		// healthRenderer->fillPercent = 0.25f;
		healthRenderer->shader = game->resources->getShaderProgram("color");
		*/

		auto camera = player->addComponent<hagame::graphics::CameraComponent>();
		camera->camera = std::make_shared<hagame::graphics::OrthographicCamera>(game->window->size);

		for (int i = 0; i < map.size(); i++) {
			for (int j = 0; j < map[i].size(); j++) {
				if (map[i][j] == '#')
					addWall(Vec2(CUBE_SIZE).prod(Vec2({ (float)j, (float)map.size() - i })), Vec2(CUBE_SIZE));
				else if (map[i][j] == 'P')
					player->transform->setPosition(Vec3(CUBE_SIZE).prod(Vec3({ (float)j, (float)map.size() - i , 0.0 })));
				else if (map[i][j] == 'T')
					addTurret(Vec2(CUBE_SIZE).prod(Vec2({ (float)j, (float)map.size() - i })));
			}
		}

		inventory->pickupWeapon(HANDGUN);
		inventory->pickupWeapon(PLASMA_RIFLE);
		inventory->pickupWeapon(RIFLE);
		inventory->pickupWeapon(SHOTGUN);
		inventory->pickupWeapon(ROCKET_LAUNCHER);
		inventory->setWeapon(PLASMA_RIFLE);

		playerRenderer = player->addComponent<PlayerRenderer>();
		playerRenderer->shader = game->resources->getShaderProgram("sprite");

		playerCollider = player->addComponent<hagame::physics::Collider>();
		playerCollider->display = true;
		playerCollider->shader = game->resources->getShaderProgram("color");
		playerCollider->type = hagame::physics::ColliderType::SphereCollider;
		playerCollider->boundingSphere = Sphere(Vec3::Zero(), (25.0f));
		playerCollider->ignoreTags.push_back("bullet");

		game->resources->getFileSystem()->forEachFile("Textures/TopDownShooter", [this](String aPath, String aName) {
			game->resources->getFileSystem()->forEachFile(aPath, [this, aName](String bPath, String bName) {

				auto spriteImages = game->resources->getFileSystem()->listFiles(bPath, [](String a, String b) {
					auto aParts = stringSplit(hagame::utils::FileSystem::GetFile(a), '_');
					auto bParts = stringSplit(hagame::utils::FileSystem::GetFile(b), '_');
					int aNum = std::stoi(aParts[aParts.size() - 1]);
					int bNum = std::stoi(bParts[bParts.size() - 1]);
					return aNum < bNum;
				});


				if (aName == "feet") {

					auto feetSprites = playerRenderer->feet->add(aName + "_" + bName);
					feetSprites->rect = Rect(Vec2::Zero(), Vec2(100.0f));

					for (auto spriteImage : spriteImages) {
						auto texture = game->resources->loadTexture(stringSplit(spriteImage, '.')[0], spriteImage);
						feetSprites->textures.push_back(texture);
					}
				}
				else {
					auto bodySprites = playerRenderer->body->add(aName + "_" + bName);
					bodySprites->rect = Rect(Vec2::Zero(), Vec2(25.0f));

					for (auto spriteImage : spriteImages) {
						auto texture = game->resources->loadTexture(stringSplit(spriteImage, '.')[0], spriteImage);
						bodySprites->textures.push_back(texture);
					}
				}
			});
		});

		playerRenderer->body->setActive("handgun_move");
		playerRenderer->feet->setActive("feet_walk");
		
		// player->transform->setPosition(Vec3({ game->window->size[0] / 2.0f, game->window->size[1] / 2.0f, 0.0f }));
	}

	void onSystemUpdate(double dt) {

		Vec2 mousePos = game->input.keyboardMouse.mouse.position;
		mousePos[1] = game->window->size[1] - mousePos[1];
		mousePos += (player->transform->position.resize<2>() - game->window->size * 0.5);
		auto delta = mousePos.resize<3>() - player->transform->position;
		auto dotProd = delta.normalized().dot(game->input.keyboardMouse.lAxis.resize<3>());

		player->transform->setRotation(Quat(atan2(-delta[1], delta[0]), Vec3::Back()));

		auto forward = player->transform->right();
		auto right = player->transform->bottom();

		playerController->update(game->input.keyboardMouse.lAxis, !game->input.keyboardMouse.b, dt);

		float t;

		auto v1 = Vec3({ playerController->vel[0], 0.0f, 0.0f });
		auto v2 = Vec3({0.0f, playerController->vel[1], 0.0f });

		if (game->collisions.checkCollisions(player, playerCollider, v1, dt, t)) {
			v1 *= t;
			player->transform->move(v1 * dt * t * 0.5);
		}
		else {
			player->transform->move(v1 * dt);
		}

		if (game->collisions.checkCollisions(player, playerCollider, v2, dt, t)) {
			v2 *= t;
			player->transform->move(v2 * dt * t * 0.5);
		}
		else {
			player->transform->move(v2 * dt);
		}

		playerController->vel = Vec3({ v1[0], v2[1], 0.0f });

		auto pixelShader = game->resources->getShaderProgram("color");
		pixelShader->use();
		pixelShader->setMVP(Mat4::Identity(), Mat4::Identity(), scene->projMat);
		// hagame::graphics::drawLine(hagame::graphics::Line(player->transform->position, mousePos.resize<3>(), hagame::graphics::Color::blue()), pixelShader);

		hagame::math::Ray ray = hagame::math::Ray(player->transform->position, (mousePos.resize<3>() - player->transform->position) * 1000000);

		hagame::graphics::drawRect(Rect(
			mousePos.resize<2>() - Vec2(5.0f),
			Vec2(10.0f)
		), hagame::graphics::Color::blue(), game->resources->getShaderProgram("color"));

		auto rayHit = game->collisions.raycast(player, ray, t, {"bullet"});
		
		if (rayHit.has_value()) {

			auto point = ray.getPointOnLine(t).resize<2>();

			auto spread = inventory->activeWeapon().value()->weapon.bullet.value().spread * 200;
			auto size = tan(spread) * (player->transform->position.resize<2>() - point).magnitude();

			/*hagame::graphics::drawRect(Rect(
				point - Vec2(size / 2),
				Vec2(size)
			), hagame::graphics::Color::red(), game->resources->getShaderProgram("color"));
			*/
			hagame::graphics::drawRect(Rect(
				point - Vec2(spread / 2),
				Vec2(spread)
			), hagame::graphics::Color::red(), game->resources->getShaderProgram("color"));

		}

		timer.reset();

		

		if (game->input.keyboardMouse.keyboard.numbersPressed[1]) {
			inventory->setWeapon(HANDGUN);
		}

		if (game->input.keyboardMouse.keyboard.numbersPressed[2]) {
			inventory->setWeapon(SHOTGUN);
		}

		if (game->input.keyboardMouse.keyboard.numbersPressed[3]) {
			inventory->setWeapon(RIFLE);
		}

		if (game->input.keyboardMouse.keyboard.numbersPressed[4]) {
			inventory->setWeapon(PLASMA_RIFLE);
		}

		if (game->input.keyboardMouse.keyboard.numbersPressed[5]) {
			inventory->setWeapon(ROCKET_LAUNCHER);
		}

		auto weapon = inventory->activeWeapon();
		auto animation = weapon.has_value() ? weapon.value()->weapon.animation : "feet";

		if (weapon.has_value()) {
			// std::cout << game->input.keyboardMouse.mouse.leftPressed << "\n";
			if ((weapon.value()->weapon.automatic && game->input.keyboardMouse.mouse.left) || 
				(!weapon.value()->weapon.automatic && game->input.keyboardMouse.mouse.leftPressed))
			{
				scene->getSystem<WeaponSystem>()->fire(player, player->transform->position, delta, weapon.value());
			}			
		}

		if (playerController->vel.magnitude() < EPSILON) {
			playerRenderer->feet->setActive("feet_idle");
			playerRenderer->body->setActive(animation + "_idle");
		}
		else if (dotProd < 0 && dotProd >= -0.5) {
			playerRenderer->feet->setActive("feet_strafe_left");
			playerRenderer->body->setActive(animation + "_move");
		}
		else if (dotProd > 0 && dotProd <= 0.5) {
			playerRenderer->feet->setActive("feet_strafe_right");
			playerRenderer->body->setActive(animation + "_move");
		}
		else {
			playerRenderer->feet->setActive("feet_run");
			playerRenderer->body->setActive(animation + "_move");
		}

		playerRenderer->shader->use();
		playerRenderer->shader->setMVP(
			Mat4::Translation(player->transform->position) * Mat4::Rotation(player->transform->rotation) * Mat4::Scale(playerRenderer->size.resize<3>()),
			Mat4::Identity(),
			scene->projMat
		);

		if (playerRenderer->feet->hasActive()) {
			playerRenderer->feet->active()->draw(playerRenderer->shader);
			playerRenderer->feet->active()->update(dt);
		}

		if (playerRenderer->body->hasActive()) {
			playerRenderer->body->active()->draw(playerRenderer->shader);
			playerRenderer->body->active()->update(dt);
		}
	}
};

#endif
