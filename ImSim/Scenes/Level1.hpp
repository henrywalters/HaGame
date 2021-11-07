#ifndef IMSIM_LEVEL_1_HPP
#define IMSIM_LEVEL_1_HPP

#include "../../HaGame/HaGame.h"

class Level1 : public hagame::Scene {
public:

	void onSceneUpdate(double dt) {
		std::cout << dt << "\n";
	}
};

#endif
