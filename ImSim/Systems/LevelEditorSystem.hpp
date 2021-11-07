#ifndef LEVEL_EDITOR_SYSTEM_HPP
#define LEVEL_EDITOR_SYSTEM_HPP

#include "../../HaGame/HaGame.h"

using namespace hagame::graphics; 
using namespace hagame::ecs;
using namespace hagame::physics;

float CAMERA_RAY_DISTANCE = 1000.0f;
int GRID_ROWS = 40;
int GRID_COLS = 40;
Vec3 CELL_SIZE = Vec3(1.0f);

class LevelEditorSystem : public hagame::ecs::System {
public:

	enum EditorMode {
		AddCube,
		Selection,
	};

	Array<String> EditorModeLabels{
		"Add cube",
		"Selection",
	};

	EditorMode editorMode = EditorMode::AddCube;

	Vec2 mousePos;
	Vec3 mouseWorldPos;
	hagame::math::Ray mouseWorldRay;
	Vec3 mouseGridPos;
	Vec3Int mouseGridPosIndex;
	Vec3 mouseGridPosNorm;

	ShaderProgram* colorShader;
	ShaderProgram* textureShader;

	Ptr<hagame::utils::SpatialMap<Entity, float>> entityMap;

	std::optional<Ptr<Entity>> selected;

	String getSystemName() {
		return "LevelEditor";
	}

	void onSystemStart() {
		entityMap = std::make_shared< hagame::utils::SpatialMap<Entity, float>>(CELL_SIZE);
	}

	void onSystemUpdate(double dt) {
		updateMouseInfo();
		updateShaders();
		drawGrid(Vec3::Zero(), CELL_SIZE.resize<2>(), GRID_ROWS, GRID_COLS, Color::blue());
		drawUI();

		switch (editorMode) {
		case EditorMode::AddCube:
			handleCubeAdding();
			break;
		case EditorMode::Selection:
			handleSelection();
			break;
		}
	}

	void drawUI() {
		ImGui::Begin("Level Editor");

		ImGui::Text(("Mouse Grid Pos: " + mouseGridPos.toString()).c_str());
		ImGui::Text(("Mouse Grid Index: " + mouseGridPosIndex.toString()).c_str());

		for (int i = 0; i < EditorModeLabels.size(); i++) {
			ImGui::RadioButton(EditorModeLabels[i].c_str(), (int*) &editorMode, i);

			if (i < EditorModeLabels.size() - 1)
				ImGui::SameLine();
		}

		if (selected.has_value()) {

			if (ImGui::DragFloat3("Position", selected.value()->transform->getPositionRef()->vector, 0.1, -10, 10)) {
				selected.value()->transform->updateModel();
			}
			auto mesh = selected.value()->getComponent<DynamicMeshRenderer>()->mesh;
			mesh->updateUI();
			selected.value()->getComponent<Collider>()->boundingCube = mesh->getMesh()->getBoundingCube();

			Rect bounds = mesh->getMesh()->getScreenBounds(selected.value()->transform->getModelMatrix(), scene->viewMat, scene->projMat, game->window->size, 5.0f);
			drawScreenRect(bounds, Color::green());
		}
	}

	void handleSelection() {
		float t;
		std::optional<Hit> rayHit = game->collisions.raycast(scene->activeCameraEntity, mouseWorldRay, t);

		if (rayHit.has_value()) {
			DynamicMeshRenderer* dMesh = rayHit.value().entity->getComponent<DynamicMeshRenderer>();
			if (dMesh != NULL) {

				if (!selected.has_value() || (selected.has_value() && selected.value()->uuid != rayHit.value().entity->uuid)) {
					Rect bounds = dMesh->mesh->getMesh()->getScreenBounds(rayHit.value().entity->transform->getModelMatrix(), scene->viewMat, scene->projMat, game->window->size, 5.0f);
					drawScreenRect(bounds, Color::red());
				}

				if (game->input.keyboardMouse.mouse.leftPressed) {
					selected = rayHit.value().entity;
				}
			}
		}
	}

	void handleCubeAdding() {

		float t;
		auto rayHit = game->collisions.raycast(scene->activeCameraEntity, mouseWorldRay, t);

		if (rayHit.has_value()) {

			auto hitPos = rayHit.value().entity->transform->getPosition();
			Vec3 newPos;
			if (hitPos == Vec3::Zero()) {
				hitPos = mouseGridPosNorm;
				newPos = hitPos;
			}
			else {
				hitPos -= CELL_SIZE * 0.5f;
				newPos = hitPos + Vec3::Top(CELL_SIZE[1]);
			}

			drawCubeOutline(
				Cube(
					hitPos + Vec3::Top(0.02f),
					Vec3(CELL_SIZE)
				),
				Color::green(),
				colorShader
			);

			if (game->input.keyboardMouse.mouse.leftPressed) {
				if (!entityMap->has(newPos)) {
					entityMap->insert(newPos, addCube(newPos));
				}
			}

			if (game->input.keyboardMouse.mouse.rightPressed && !game->input.keyboardMouse.keyboard.lCtrl) {
				if (entityMap->has(hitPos)) {
					for (auto cube : entityMap->get(hitPos)) {
						scene->removeEntity(cube);
					}
					entityMap->remove(hitPos);
				}
			}
		}
	}

	void updateMouseInfo() {
		mousePos = game->input.keyboardMouse.mouse.position;
		mouseWorldPos = MouseToWorld(scene->viewMat, scene->projMat, mousePos, game->window->size);
		mouseWorldRay.origin = scene->activeCameraEntity->transform->getPosition();
		mouseWorldRay.direction = (mouseWorldPos - scene->activeCameraEntity->transform->getPosition()).normalized() * CAMERA_RAY_DISTANCE;
		mouseGridPosNorm = mouseGridPosIndex.cast<float>().prod(CELL_SIZE);

		float t;
		std::optional<hagame::math::Ray::Hit> rayHit = mouseWorldRay.checkCube(Cube(Vec3({ -(float)GRID_COLS / 2.0f, 0.0f, -(float)GRID_COLS / 2.0f }), Vec3({ (float)GRID_COLS, 0.0f, (float)GRID_ROWS })), t);

		if (rayHit.has_value()) {
			mouseGridPos = rayHit.value().position;
			mouseGridPosIndex = Vec3Int({
				(int)floor(mouseGridPos[0] / CELL_SIZE[0]),
				0,
				(int)floor(mouseGridPos[2] / CELL_SIZE[2])
				});
		}
	}

	void updateShaders() {
		colorShader = game->resources->getShaderProgram("color");
		textureShader = game->resources->getShaderProgram("texture");

		updateShader(colorShader);
		updateShader(textureShader);
	}

	void updateShader(ShaderProgram * shader) {
		shader->use();
		shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
		shader->setVec3("viewPos", scene->activeCameraEntity->transform->getPosition());
	}

	Ptr<Entity> addCube(Vec3 pos) {
		auto cube = scene->addEntity();
		auto meshRenderer = cube->addComponent<DynamicMeshRenderer>();
		meshRenderer->mesh = std::make_shared<RectPrism>(CELL_SIZE);
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();
		auto collider = cube->addComponent<Collider>();
		collider->boundingCube = Cube(CELL_SIZE * -0.5f, CELL_SIZE);

		cube->transform->move(pos + CELL_SIZE * 0.5f);

		return cube;
	}

	void drawScreenRect(Rect rect, Color color) {
		auto shader = game->resources->getShaderProgram("color");
		shader->use();
		shader->setMVP(Mat4::Identity(), Mat4::Identity(), Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1));
		drawRect(rect, color, shader);
	}

	void drawGrid(Vec3 pos, Vec2 cellSize, int rows, int cols, Color color) {

		Vec3 gridSize = Vec3({ cellSize[0] * cols, 0.0f, cellSize[1] * rows });
		Vec3 min = Vec3({ pos[0] - cellSize[0] * (float)cols * 0.5f, pos[1], pos[2] - cellSize[1] * (float)rows * 0.5f });
		for (int i = 0; i <= rows; i++) {
			drawLine(
				Line(min + Vec3({ 0, 0, cellSize[1] * i }), min + Vec3({ gridSize[0], 0.0f, cellSize[1] * i }), color),
				colorShader
			);
		}

		for (int j = 0; j <= cols; j++) {
			drawLine(
				Line(min + Vec3({ cellSize[0] * j, 0, 0.0f }), min + Vec3({ cellSize[0] * j, 0, gridSize[2] }), color),
				colorShader
			);
		}
	}
};

#endif