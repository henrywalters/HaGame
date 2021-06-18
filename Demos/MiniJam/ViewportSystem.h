#ifndef VIEWPORT_SYSTEM_H
#define VIEWPORT_SYSTEM_H

#include "../../HaGame/HaGame.h"

class ViewportSystem : public hagame::ecs::System {

	

public :

	Vec2 speed = Vec2(200.0);
	Mat4 projection;
	Vec2 offset;

	String getSystemName() {
		return "ViewPortSystem";
	}

	void onSystemBeforeUpdate(double dt) {
		//projection = Mat4::Orthographic()
		offset += game->input.keyboardMouse.dPad.prod(speed) * dt;
		projection = Mat4::Orthographic(offset[0], game->window->size[0] + offset[0], offset[1], game->window->size[1] + offset[1], 0, 1);
		//std::cout << offset.toString() << "\n";
	}
};

#endif
