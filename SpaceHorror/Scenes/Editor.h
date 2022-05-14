#ifndef SH_EDITOR_H
#define SH_EDITOR_H

#include "./../../HaGame/HaGame.h"
#include "./../Common/Colors.h"

using namespace hagame::ecs;
using namespace hagame::graphics;
using namespace hagame::ui;

class Editor : public hagame::Scene {
public:

	void onSceneInit();

	void onSceneUpdate(double dt);

private:

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

	Tools selectedTool = Tools::TileEditor;

	const char* CONTROL_SETTINGS = "Control Settings";

	const Vec2 GRID_SIZE = Vec2(1000.0f);

	float zoomSpeed = 10.0f;
	float cameraSpeed = 100.0f;

	Vec2 worldPos;
	Vec2 mousePos;
	Vec2 dragStartPos;
	Vec2 startOffset;
	bool isDragging = false;

	RawPtr<Entity> cameraEntity;
	Ptr<OrthographicCamera> orth;

	RawPtr<Entity> editorEntity;

	void updateCamera(double dt);

	void updateGrid();

	void render();

	// Entry point for all the tools used
	void renderGUI();

	void renderMenuBar();

	void renderToolBox();

	void renderSelectedTool();

	void renderControlSettings();

	void tileEditor();

	void enemyEditor();

	void fxEditor();

	void connector();

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