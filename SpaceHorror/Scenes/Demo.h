#ifndef DEMO_H
#define DEMO_H

#include "./../../HaGame/HaGame.h"
#include "./../../HaGame/UI/EntityTree.h"

#include "./../Common/RuntimeLevel.h"
#include "./../Common/Weapons.h"
#include "./../Common/Colors.h"

#include "./../Systems/PlatformerSystem.h"
#include "./../Systems/AISystem.h"
#include "./../Systems/CustomRenderSystem.h"
#include "./../Systems/WeaponSystem.h"
#include "./../Systems/HealthSystem.h"

#include "./../Components/PlayerController.h"
#include "./../Components/AI/Walker.h"
#include "./../Components/HealthBarRenderer.h"
#include "./../Components/WeaponController.h"

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

	RawPtr<hagame::ecs::Entity> light;
	RawPtr<hagame::ecs::Entity> player;

	Array<RawPtr<hagame::ecs::Entity>> walkers;

	hagame::ui::EntityTree entityTree;

	Color lColor = Color::white();
	float lConst, lLin, lQuad = 1.0;

	bool guiActive();

	const Array<String> MAP = {
		"############################",
		"############################",
		"############################",
		"############################",
		"#------------------------- #",
		"#-L----#####################",
		"# ---P--- -----------------#",
		"#--------------------------#",
		"#--------------------------#",
		"#--------------------------#",
		"#--------------------------#",
		"############################",
	};

};

#endif
