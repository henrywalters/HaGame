#ifndef IM_SIM_LEVEL_3
#define IM_SIM_LEVEL_3

#include "../../HaGame/HaGame.h"
#include "../Systems/MouseSystem.hpp"
#include "../Systems/StateSystem.hpp"
#include "../Systems/Movement.hpp"
#include "../Systems/ProjectileSystem.hpp"
#include "../Systems/GizmoSystem.hpp"
#include "../Components/PlayerMovement.hpp"
#include "../Common/RuntimeLevel.hpp"
#include "../Components/Player.hpp"

using namespace hagame::ecs;
using namespace hagame::graphics;

class Level3 : public RuntimeLevel {
public:
	Ptr<Entity> light;
	Ptr<CameraSystem> cameraSystem;

	void onSceneInit() {
		addSystem<RenderSystem>();
		addSystem<LightingSystem>();
		cameraSystem = addSystem<CameraSystem>();
	}
};

#endif
