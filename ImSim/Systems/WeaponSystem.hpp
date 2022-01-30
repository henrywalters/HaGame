#ifndef WEAPON_SYSTEM_HPP
#define WEAPON_SYSTEM_HPP

#include "../../HaGame/HaGame.h"

class WeaponSystem : public hagame::ecs::System {
public:

	String getSystemName() {
		return "ImSim_Weapon_System";
	}


};

#endif