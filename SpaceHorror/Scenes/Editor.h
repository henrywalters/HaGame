#ifndef SH_EDITOR_H
#define SH_EDITOR_H

#include "./../../HaGame/HaGame.h"

using namespace hagame::ecs;
using namespace hagame::graphics;
using namespace hagame::ui;

class Editor : public hagame::Scene {
public:

	void onSceneActivate() {

	}

	void onSceneUpdate(double dt) {
		if (game->input.keyboardMouse.startPressed) {
			game->scenes.activate("home");
		}
	}
};

#endif