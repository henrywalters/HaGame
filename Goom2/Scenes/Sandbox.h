#ifndef GOOM_SANDBOX_H
#define GOOM_SANDBOX_H

#include "../../HaGame/HaGame.h"
#include "./Components/PlayerController.h"
#include "./../Systems/PlayerSystem.h"
#include "./../Systems/WeaponSystem.h"
#include "./../Systems/EnemySystem.h"

class Sandbox : public hagame::Scene {

	void onSceneInit() {
		std::cout << "Initializing Sandbox\n";

		addSystem<PlayerSystem>();
		addSystem<WeaponSystem>();
		addSystem<EnemySystem>();
	}
};

#endif