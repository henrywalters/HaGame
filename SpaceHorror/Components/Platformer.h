#ifndef PLATFORMER_H
#define PLATFORMER_H

struct Platformer {
	float groundDrag = 1000.0f;
	float airDrag = 50.0f;
	bool grounded = false;
};

#endif