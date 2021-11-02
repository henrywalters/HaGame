#ifndef BULLET_H
#define BULLET_H

#include <optional>
#include <string>

struct ProjectileDef {
	std::string sprite;
	Vec2 size;
};

struct Bullet {
	float damage;
	float speed;
	float spread;
	float blastRadius;
	bool explodes;
	std::optional<ProjectileDef> projectile;
};

const Bullet HANDGUN_BULLET{
	10.0f,
	8.0f,
	5.0f * DEG_TO_RAD,
	0.0f,
	false,
	std::nullopt
};

const Bullet SHOTGUN_PELLET{
	8.0f,
	10.0f,
	15.0f * DEG_TO_RAD,
	0.0f,
	false,
	std::nullopt
};

const Bullet RIFLE_BULLET{
	17.5f,
	10.0f,
	5.0f * DEG_TO_RAD,
	0.0f,
	false,
	std::nullopt
};

const Bullet PLASMA_BLOB{
	20.0f,
	30.0f,
	5.0f * DEG_TO_RAD,
	0.0f,
	false,
	ProjectileDef{
		"plasma_ball",
		Vec2({0.1, 0.1})
	}
};

const Bullet ROCKET{
	100.0f,
	5.0f,
	0.0f * DEG_TO_RAD,
	20.0f,
	true,
	ProjectileDef{
		"missle",
		Vec2({163, 73}) * 0.25f
	}
};

#endif
