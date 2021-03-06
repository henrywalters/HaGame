#include "Collisions.h"

bool hagame::physics::isColliding(Rect r1, Rect r2)
{
	return r1.contains(r2.pos) ||
		r1.contains(r2.pos + Vec2({ r2.size[0] })) ||
		r1.contains(r2.pos + Vec2({ 0.0f, r2.size[1] })) ||
		r1.contains(r2.pos + r2.size);
}
