#ifndef DEMO_H
#define DEMO_H

#include "./../../HaGame/HaGame.h"
#include "./../Common/RuntimeLevel.h"
#include "./../Systems/PlatformerSystem.h"
#include "./../Components/PlayerController.h"
#include "./../Common/Weapons.h"

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

	Ptr<hagame::ecs::Entity> player;

	std::vector<Bullet> bullets;
	std::vector<Weapon> weapons;

	const Array<String> MAP = {
		"############################",
		"#--------------------------#",
		"#---------#--#--#----------#",
		"#-----P--------------------#",
		"# ----------------####-----#",
		"#--------------------------#",
		"#----------------------##--#",
		"#--------------------------#",
		"############################",
	};

};

#endif
