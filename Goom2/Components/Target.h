#ifndef G_TARGET_H
#define G_TARGET_H

#include "./../../HaGame/HaGame.h"
#include <functional>

struct Target {
	virtual Vec3 calcPosition(Vec3 currPos, double dt) {
		return Vec3::Zero();
	}
};

#endif
