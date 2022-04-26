#ifndef RUNTIME_LEVEL_H
#define RUNTIME_LEVEL_H

#include "./../../HaGame/HaGame.h"
#include "./../Common/Colors.h"
#include "./../Common/Weapons.h"
#include "./../Components/Platformer.h"
#include "./../../HaGame/Utils/ConfigParser.h"

class RuntimeLevel : public hagame::Scene {
protected:

	const String PRIMARY_FONT = "Fonts/horroroid/horroroid.ttf";
	const String SECONDARY_FONT = "Fonts/arial.ttf";

	const float PIXELS_PER_METER = 100.0f;
	const float WIDTH_HEIGHT_RATIO = 1.3f;

	const Vec2 PLAYER_SIZE = Vec2(1.0f);
	const Vec2 BLOCK_SIZE = Vec2(1.5f);
	const Vec2 WALKER_SIZE = Vec2(0.25);

	Vec2 cellSize = Vec2(1.0f);

	float lookAngle = 0;
	Vec2 mousePos;

	RawPtr<hagame::ecs::Entity> hud;

	RawPtr<hagame::ecs::Entity> camera;
	Ptr<hagame::graphics::OrthographicCamera> orth;

	Ptr<hagame::graphics::OrthographicCamera> uiOrth;

	RawPtr<hagame::ecs::Entity> addCamera();
	RawPtr<hagame::ecs::Entity> addHUD();
	RawPtr<hagame::ecs::Entity> RuntimeLevel::addQuad(Vec3 pos, Vec2 size, Color color);
	RawPtr<hagame::ecs::Entity> addSprite(String textureName, Vec2 pos, Vec2 size = Vec2(1.0f));
	void addPhysics(RawPtr<hagame::ecs::Entity> entity, float mass);
	hagame::physics::Collider* addBoxCollider(RawPtr<hagame::ecs::Entity> entity, Vec2 size, bool dynamic = true);
	hagame::physics::Collider* addCircleCollider(Ptr<hagame::ecs::Entity> entity, float radius, bool dynamic = true);
	

	void setMousePos(Vec2 rawMousePos);
	void setWindowSize(Vec2 size);

private:

	// Return the time <= dt such that the entity cube is just less than colliding with the other cube.
	float binarySearchCollisionTime(Cube entityCube, Cube otherCube, Vec2 velocity, double dt);
};



#endif
