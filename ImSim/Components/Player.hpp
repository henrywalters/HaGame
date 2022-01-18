#ifndef IMSIM_PLAYER_HPP
#define IMSIM_PLAYER_HPP

#include "../../HaGame/HaGame.h"

using namespace hagame::ecs;

struct Player {
	Ptr<Entity> player;
	Ptr<Entity> body;
	Ptr<Entity> broadCollider;
	Ptr<Entity> groundCollider;
	Ptr<Entity> camera;
	Ptr<Entity> gun;
};

#endif
