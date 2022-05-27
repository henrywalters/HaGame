#ifndef SH_EDITOR_H
#define SH_EDITOR_H

#include "./../../HaGame/HaGame.h"
#include "./../Common/Colors.h"
#include "./../Common/Config.h"
#include "./../../HaGame/UI/ResourceBrowser.h"
#include "./../../HaGame/Utils/Grid.h"

using namespace hagame::ecs;
using namespace hagame::graphics;
using namespace hagame::ui;

class Editor : public hagame::Scene {
public:

	void onSceneInit();

	void onSceneUpdate(double dt);

private:

	struct Error {
		int code;
		String message;
	};

	struct Tile {
		String name;
		hagame::graphics::Texture* texture;
		float opacity;
		float zIndex;

		hagame::utils::Grid<RawPtr<Entity>> placements;
	};

	Array<Ptr<Tile>> tiles;
	Map<String, int> tileMap;

	int selectedTileIdx = 0;

	enum class Tools {
		TileEditor,
		EnemyEditor,
		FxEditor,
		Connector,
	};

	const Array<String> TOOL_NAMES = {
		"Tile Editor",
		"Enemy Editor",
		"FX Editor",
		"Connector"
	};

	Optional<hagame::utils::File> activeFile;

	Tools selectedTool = Tools::TileEditor;

	const char* CONTROL_SETTINGS = "Control Settings";
	const char* NEW_LEVEL = "New Level";

	const Vec2 GRID_SIZE = Vec2(1000.0f);

	float zoomSpeed = 10.0f;
	float cameraSpeed = 100.0f;

	Vec2Int gridPartitions = Vec2Int(50, 50);
	float cellSize = 50.0f;

	Vec2 worldPos;
	Vec2 mousePos;
	Vec2 dragStartPos;
	Vec2 startOffset;
	bool isDragging = false;

	RawPtr<Entity> cameraEntity;
	Ptr<OrthographicCamera> orth;

	RawPtr<Entity> editorEntity;

	ResourceBrowser resourceBrowser;

	Config config;

	void updateCamera(double dt);

	void updateGrid();

	void render();

	bool guiActive();

	// Entry point for all the tools used
	void renderGUI();

	// File operations
	void newLevel();
	Optional<Error> saveLevel();
	Optional<Error> loadLevel();

	// Sub UI methods
	void renderMenuBar();
	void renderToolBox();
	void renderSelectedTool();
	void renderControlSettings();
	void renderNewLevel();
	void renderSaveLevelAs();
	void renderLoadLevel();

	// Tool methods - add code here for functionality!
	void tileEditor();
	void enemyEditor();
	void fxEditor();
	void connector();

	// Helper functions
	void addTile(int tileIdx, Vec2Int idx);
	void loadTiles();

	RawPtr<Entity> addQuad(Vec3 pos, Vec2 size, Color color) {
		auto entity = addEntity();
		entity->move(pos);
		auto quad = entity->addComponent<QuadRenderer>(size);
		quad->quad->setOrigin(Vec2::Zero());
		quad->color = color;
		quad->shader = game->resources->getShaderProgram("color");
		return entity;
	}
};

#endif