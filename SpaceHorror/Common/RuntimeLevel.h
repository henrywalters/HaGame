#ifndef RUNTIME_LEVEL_H
#define RUNTIME_LEVEL_H

#include "./../../HaGame/HaGame.h"

class RuntimeLevel : public hagame::Scene {
protected:
	const float PIXELS_PER_METER = 100.0f;
	const float WIDTH_HEIGHT_RATIO = 1.3f;

	const Vec2 PLAYER_SIZE = Vec2(1.0f);
	const Vec2 BLOCK_SIZE = Vec2(1.5f);

	Vec2 cellSize = Vec2(1.0f);

	Vec2 mousePos;

	Ptr<hagame::ecs::Entity> camera;
	Ptr<hagame::graphics::OrthographicCamera> orth;

	Ptr<hagame::ecs::Entity> addCamera();
	Ptr<hagame::ecs::Entity> addSprite(String textureName, Vec2 pos, Vec2 size = Vec2(1.0f));
	void addPhysics(Ptr<hagame::ecs::Entity> entity, float mass);
	hagame::physics::Collider* addBoxCollider(Ptr<hagame::ecs::Entity> entity, Vec2 size, bool dynamic = true);
	hagame::physics::Collider* addCircleCollider(Ptr<hagame::ecs::Entity> entity, float radius, bool dynamic = true);

	// Return the allowable velocity after collisions
	void resolveCollisions(Ptr<hagame::ecs::Entity> entity, Vec3& velocity, bool& grounded);

	void setMousePos(Vec2 rawMousePos);
	void setWindowSize(Vec2 size);

private:

	struct EntityHit {
		Ptr<Entity> entity;
		Vec3 normal;
		float t;
	};

	// Return the time <= dt such that the entity cube is just less than colliding with the other cube.
	float binarySearchCollisionTime(Cube entityCube, Cube otherCube, Vec2 velocity, double dt);
};



#endif
