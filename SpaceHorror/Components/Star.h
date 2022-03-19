#ifndef HS_STAR_H
#define HS_STAR_H

#include "./../../HaGame/HaGame.h"

struct Star {
	Color color = Color(1.0f, 1.0f, 1.0f, 0.0f);
	float maxBrightness = 1.0f;
	float period = 1.0f;
	float offset = 0.0f;
	float t = 0;

	void randomize(float maximum = 1.0f) {
		hagame::utils::Random random;
		maxBrightness = random.real<double>(0.0, maximum);
		period = random.real<double>(0.1, 3);
		offset = random.real<double>(0, 1);
	}

	void update(double dt) {
		t += dt;
		color[3] = (maxBrightness * 0.5f * sin((t / period) + offset)) + (maxBrightness * 0.5f);
	}
};

#endif
