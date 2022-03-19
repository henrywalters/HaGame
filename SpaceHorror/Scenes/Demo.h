#ifndef DEMO_H
#define DEMO_H

#include "./../../HaGame/HaGame.h"

#include "./../Common/RuntimeLevel.h"
#include "./../Common/Weapons.h"
#include "./../Common/Colors.h"

#include "./../Systems/PlatformerSystem.h"
#include "./../Systems/AISystem.h"

#include "./../Components/PlayerController.h"
#include "./../Components/AI/Walker.h"

class Demo : public RuntimeLevel {
public:
	void onSceneInit();
	void onSceneActivate();
	void onSceneBeforeUpdate();
	void onSceneUpdate(double dt);
	void onSceneAfterUpdate();
	void onSceneDeactivate();

private:

	void renderPlayerConfig();

	double frameTime;

	RawPtr<hagame::ecs::Entity> player;

	std::vector<Bullet> bullets;
	std::vector<Weapon> weapons;

	Array<RawPtr<hagame::ecs::Entity>> walkers;

	const Array<String> MAP = {
		"############################",
		"# ----------    -----------#",
		"#---------#--#--#----------#",
		"#--- -P--------------------#",
		"# ----------------####-----#",
		"#--------------------------#",
		"#----------------------##--#",
		"#--W-----------------------#",
		"############################",
	};

};

#endif
