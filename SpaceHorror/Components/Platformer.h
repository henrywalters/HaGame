#ifndef PLATFORMER_H
#define PLATFORMER_H

struct Platformer {
	float groundDrag = 600.0f;
	float airDrag = 500.0f;
	bool grounded = false;
};

#endif