#ifndef GOOM_MAIN_MENU_H
#define GOOM_MAIN_MENU_H

#include "../../HaGame/HaGame.h"

class MainMenu: public hagame::Scene {
	void onSceneUpdate(double dt) {
		std::cout << dt << std::endl;
	}
};

#endif