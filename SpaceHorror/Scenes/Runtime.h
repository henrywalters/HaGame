#ifndef SH_RUNTIME_H
#define SH_RUNTIME_H

#include "./../../HaGame/HaGame.h"
#include "./../../HaGame/UI/EntityTree.h"
#include "./../../HaGame/Graphics/Components/OrthographicCameraDragController.hpp"
#include "./../../HaGame/Utils/TileGrid.h"

#include "./../Common/RuntimeLevel.h"
#include "./../Common/Weapons.h"
#include "./../Common/Colors.h"
#include "./../Common/Config.h"

#include "./../Systems/PlatformerSystem.h"
#include "./../Systems/AISystem.h"
#include "./../Systems/CustomRenderSystem.h"
#include "./../Systems/WeaponSystem.h"
#include "./../Systems/HealthSystem.h"

#include "./../Components/PlayerController.h"
#include "./../Components/AI/Walker.h"
#include "./../Components/HealthBarRenderer.h"
#include "./../Components/WeaponController.h"

struct RuntimeEntity {
	RawPtr<hagame::ecs::Entity> entity;
	Vec3 initialPos;
};

class Runtime : public RuntimeLevel {
public:

	void onSceneInit();
	void onSceneActivate();
	void onSceneBeforeUpdate();
	void onSceneUpdate(double dt);
	void onSceneAfterUpdate();
	void onSceneDeactivate();

private:

	// General variables
	Config m_config;
	RawPtr<hagame::utils::ConfigFile> m_gameConfig;
	bool m_devMode = false;

	RawPtr<hagame::ecs::Entity> m_previewer;
	RawPtr<hagame::ecs::Entity> m_player;

	// Level variables
	double m_startTime;
	RawPtr<hagame::ecs::Entity> m_playerStart = NULL;

	float edgeThickness = 0.1;
	Color edgeColor = Color::blue();
	bool placingEdge = false;
	hagame::math::Line currentEdge;
	
	//Array<RuntimeEntity> m_tiles;
	hagame::utils::TileGrid<RuntimeEntity> m_tiles = hagame::utils::TileGrid<RuntimeEntity>(BLOCK_SIZE);
	Array<Polygon2D> m_tileShapes;
	Array<RuntimeEntity> m_lights;
	Array<hagame::math::Line> m_edges;

	Array<hagame::utils::TileGrid<RuntimeEntity>::Edge> edges;

	// Utilities

	// Load a level from a file name
	void loadLevel(String level);

	// Initialize all the editor tools used
	void initializeEditorTools();

	// Set the current preview icon to a certain texture
	void setPreviewer(RawPtr<Texture> texture, Vec2 size);

	Array<Vec2> computeLineOfSight(Vec2 pos);

	void renderDeveloperWindow();

	void play();
	void stop();
	void reset();
};

#endif
