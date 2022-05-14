#ifndef BULLET_HOLE_H
#define BULLET_HOLE_H

struct BulletHole {
	float createdAt;
	double aliveFor = 1.0f;

	BulletHole(double _createdAt) {
		createdAt = _createdAt;
	}
};

#endif
