#ifndef MOVING_PLATFORM_H
#define MOVING_PLATFORM_H

#include "./../../HaGame/HaGame.h"

enum class PlatformDirection {
	Stationary, // The platform won't move at all
	Forward, // The platform is moving towards the end point
	Backward, // The platform is moving towards the start point
};

enum class PlatformMode {
	Continuous, // The platform will continue back and forth from start to end point
	Trigger, // The platform will stop once it has hit the start or end point. Calling trigger will cause the platform to move again
};

// MovingPlatform component will cause the entity to oscillate at a fixed speed between the start and end point
struct MovingPlatform {
	PlatformDirection direction = PlatformDirection::Forward;
	PlatformMode mode = PlatformMode::Continuous;
	Vec3 start, end;
	double t = 0.0f;
	float speed = 0.25f;

	Vec3 getCurrentPos() {
		return start + (end - start) * t;
	}

	void update(double dt) {
		if (direction == PlatformDirection::Stationary) return;

		auto tNew = t + speed * (direction == PlatformDirection::Forward ? dt : -dt);

		if (tNew >= 1.0f) {
			if (mode == PlatformMode::Continuous) {
				t = 2.0f - tNew;
				direction = PlatformDirection::Backward;
			}
			else {
				t = 1.0f;
				direction = PlatformDirection::Stationary;
			}
		}
		else if (tNew <= 0.0f) {
			if (mode == PlatformMode::Continuous) {
				t = -tNew;
				direction = PlatformDirection::Forward;
			}
			else {
				t = 0.0f;
				direction = PlatformDirection::Stationary;
			}
		}
		else {
			t = tNew;
		}
	}

	void trigger() {
		// Can only trigger when in trigger mode and at the start or end
		if (mode != PlatformMode::Trigger || t != 0.0f || t != 1.0f) return;

		direction = t == 0.0f ? PlatformDirection::Forward : PlatformDirection::Backward;
	}
};

#endif
